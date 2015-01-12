/**
 *  File.cpp
 *
 *  Implementation file for the File class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Namespace
 */
namespace Php {

/**
 *  Constructor
 * 
 *  The constructor receives a filename as parameter. It uses the normal
 *  PHP include path resolve algorithms to find the location of the file. 
 * 
 *  @param  name        the filename
 *  @param  size        length of the filename
 */
File::File(const char *name, size_t size)
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();
    
    // resolve the path
    _path = zend_resolve_path(name, size TSRMLS_CC);
    
    // the resolve-path function sometimes returns the original pointer, we
    // do not want that because we may have to store the pathname in this object
    if (_path != name) return;
    
    // make a full copy of the pathname
    _path = estrndup(name, size);
}

/**
 *  Destructor
 */
File::~File()
{
    // clean up path name
    if (_path) efree(_path);
    
    // clean up opcodes
    if (_opcodes) delete _opcodes;
}

/**
 *  Compile the file
 *  @return bool
 */
bool File::compile()
{
    // never works if the path is invalid
    if (!_path) return false;
    
    // is the file already compiled?
    if (_opcodes) return _opcodes->valid();
    
    // we are going to open the file
    zend_file_handle fileHandle;

    // we need the tsrm_ls variable (@todo would it be better if this was a member?)
    TSRMLS_FETCH();

    // open the file
    if (zend_stream_open(_path, &fileHandle TSRMLS_CC) == FAILURE) return false;

    // make sure the path name is stored in the handle
    if (!fileHandle.opened_path) fileHandle.opened_path = estrdup(_path);
    
    // we need temporary compiler options
    CompilerOptions options(ZEND_COMPILE_DEFAULT TSRMLS_CC);
    
    // create the opcodes
    _opcodes = new Opcodes(zend_compile_file(&fileHandle, ZEND_INCLUDE TSRMLS_CC) TSRMLS_CC);

    // close the file handle
    zend_destroy_file_handle(&fileHandle TSRMLS_CC);
    
    // done
    return _opcodes->valid();
}

/**
 *  Does the file exist?
 *  @return boolean
 */
bool File::exists()
{
    // it is of course not valid if the path could not be resolved
    if (!_path) return false;
    
    // if we have valid opcodes, we're sure that it exists
    if (_opcodes && _opcodes->valid()) return true;
    
    // retrieve stats
    struct stat buf;
    return stat(_path, &buf) == 0;
}

/**
 *  Is this a valid file?
 *  @return boolean
 */
bool File::valid()
{
    // check if file is compilable
    return compile();
}

/**
 *  Execute the file
 *  @return Value
 */
Value File::execute() 
{
    // do we already have the opcodes?
    if (_opcodes) return _opcodes->execute();

    // try compiling the file
    if (!compile()) return nullptr;

    // we need the tsrm_ls variable (@todo would it be better if this was a member?)
    TSRMLS_FETCH();

    // add the entry to the list of included files
    zend_hash_add_empty_element(&EG(included_files), _path, ::strlen(_path) + 1);
    
    // execute the opcodes
    return _opcodes->execute();
}

/**
 *  Execute a file only once
 *  @return Value
 */
Value File::once() 
{
    // skip if the path is invalid
    if (!_path) return nullptr;

    // we need the tsrm_ls variable (@todo would it be better if this was a member?)
    TSRMLS_FETCH();
    
    // check if this file was already included
    if (zend_hash_exists(&EG(included_files), _path, ::strlen(_path) + 1)) return nullptr;

    // execute the file
    return execute();
}

/**
 *  End of namespace
 */
}

