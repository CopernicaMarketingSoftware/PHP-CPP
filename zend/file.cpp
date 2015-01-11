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
 *  Execute the file
 *  @return Value
 */
Value File::execute() 
{
    // do we already have the opcodes?
    if (_opcodes) return _opcodes->execute();

    // skip if there is no valid path
    if (!_path) return nullptr;

    // we are going to open the file
    zend_file_handle fileHandle;

    // open the file
    if (zend_stream_open(_path, &fileHandle TSRMLS_CC) == FAILURE) return nullptr;

    // make sure the path name is stored in the handle
    if (!fileHandle.opened_path) fileHandle.opened_path = estrdup(_path);
    
    // we need temporary compiler options
    CompilerOptions options(ZEND_COMPILE_DEFAULT);
    
    // we need the tsrm_ls variable
    TSRMLS_FETCH();
    
    // create the opcodes
    _opcodes = new Opcodes(zend_compile_file(&fileHandle, ZEND_INCLUDE TSRMLS_CC));

    // close the file handle
    zend_destroy_file_handle(&fileHandle TSRMLS_CC);

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
    // skip if the opcodes are already known (then we know for sure that the 
    // file was already executed), also leap out if the filename was not even valid
    if (_opcodes || !_path) return nullptr;
    
    // check if this file was already included
    if (zend_hash_exists(&EG(included_files), _path, ::strlen(_path) + 1)) return nullptr;

    // execute the file
    return execute();
}

/**
 *  End of namespace
 */
}

