/**
 *  Eval.cpp
 *
 *  This file holds the implementation for the Php::eval() function
 * 
 *  @author andot <https://github.com/andot>
 *  @author tugrul <https://github.com/tugrul>
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Open PHP namespace
 */
namespace Php {

/**
 *  Evaluate a PHP string
 *  @param  phpCode     The PHP code to evaluate
 *  @return Value       The result of the evaluation
 */
Value eval(const std::string &phpCode) 
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // the current exception
    zval* oldException = EG(exception);

    // the return zval
    zval* retval = nullptr;
    MAKE_STD_ZVAL(retval);
    
    if (zend_eval_stringl_ex((char *)phpCode.c_str(), (int32_t)phpCode.length(), retval, (char *)"", 1 TSRMLS_CC) != SUCCESS)
    {
        // Do we want to throw an exception here? The original author
        // did, but there are some reasons not to:
        //
        //  1. the PHP eval() function also does not throw exceptions.
        //
        //  2. the zend_eval_string() function already triggers a 
        //     'PHP parse error' when an error occurs, which also has
        //     to be handled. If we also throw an exception here, the
        //     user will have to write two error checks: for the error
        //     and the exception.
        //
        // if we _do_ want to throw an exception, we will first have to
        // prevent the original zend_error to occur, and then turn it
        // into an exception. An exception would be nicer from a C++
        // point of view, but because of the extra complexity, we do not
        // this for now.
        return nullptr;
    }
    else
    {
        // was an exception thrown inside the eval()'ed code? In that case we 
        // throw a C++ new exception to give the C++ code the chance to catch it
        if (oldException != EG(exception) && EG(exception)) throw OrigException(EG(exception) TSRMLS_CC);

        // no (additional) exception was thrown
        return retval ? Value(retval) : nullptr;
    }
}

Value executeFile(const std::string execution, ExecutionType execType)
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();
    
    // resolve relative path to include path if exists
    char *resolvedPath = zend_resolve_path(execution.c_str(), execution.length() TSRMLS_CC);
    
    int type = (int) execType;
    
    if (execType == ExecutionType::IncludeOnce || execType == ExecutionType::RequireOnce) {
        // if the file is in include_path and included before 
        if (resolvedPath != nullptr && zend_hash_exists(&EG(included_files), resolvedPath, ::strlen(resolvedPath) + 1)) {
            return false;
        }

        type /= 2; 
    }
    
    if (resolvedPath == nullptr) {
        // use original path instead of non resolved path
        resolvedPath = estrdup(execution.c_str());
    }

    zend_file_handle fileHandle;
    
    if (zend_stream_open(resolvedPath, &fileHandle TSRMLS_CC) == FAILURE) {
        efree(resolvedPath);
        return false;
    }

    if (!fileHandle.opened_path) {
        fileHandle.opened_path = estrdup(resolvedPath);
    }

    // mark opened file as included
    if (zend_hash_add_empty_element(&EG(included_files), fileHandle.opened_path, ::strlen(fileHandle.opened_path) + 1) == FAILURE) {
        if (execType == ExecutionType::IncludeOnce || execType == ExecutionType::RequireOnce) {
            efree(resolvedPath);
            zend_file_handle_dtor(&fileHandle TSRMLS_CC); 
            return false;
        }
    }
    
    zend_uint compilerOptions = CG(compiler_options);
    
    CG(compiler_options) = ZEND_COMPILE_DEFAULT;
    // compile file and keep op_array for execution
    zend_op_array *opArray = zend_compile_file(&fileHandle, type TSRMLS_CC); 
    CG(compiler_options) = compilerOptions;
    
    efree(resolvedPath);
    
    if (EG(exception) != nullptr) {
        throw OrigException(EG(exception) TSRMLS_CC);
    }
    
    zval *retVal = nullptr;

    // keep globals of previous stack frame
    zval **origRetVal = EG(return_value_ptr_ptr);
    zend_op **origOpline = EG(opline_ptr);
    zend_op_array *origOpArray = EG(active_op_array);

    // define globals of current stack frame
    EG(return_value_ptr_ptr) = &retVal;
    EG(active_op_array) = opArray;
    
    // build local symbol table if not exists
    if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
    }
    
    // execute the kept op_array
    zend_execute(opArray TSRMLS_CC);
    
    destroy_op_array(opArray TSRMLS_CC);
    efree(opArray);
    
    // give back globals of previous stack frame
    EG(return_value_ptr_ptr) = origRetVal;
    EG(opline_ptr) = origOpline;
    EG(active_op_array) = origOpArray;
    
    // populate exception if there was from execution
    if (EG(exception) != nullptr) { 
        throw OrigException(EG(exception) TSRMLS_CC);
    }
    
    return retVal ? Value(retVal) : nullptr; 
}


/**
 *  End of namespace
 */
}
