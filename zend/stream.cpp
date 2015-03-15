/**
 *  Stream.cpp
 *
 *  Implementation file for streams
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 Copernica BV
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Utility function to open a file
 *  @param  fp
 *  @param  mode
 *  @return php_stream*
 */
static php_stream *open(FILE *fp, const char *mode)
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();
    
    // open the file
    return php_stream_fopen_from_file(fp, mode);
}

/**
 *  Utility function to open a file
 *  @param  filename
 *  @param  mode
 *  @return php_stream*
 */
static php_stream *open(const char *filename, const char *mode)
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();
    
    // open the file
    return php_stream_open_wrapper((char *)filename, (char *)mode, IGNORE_PATH, nullptr);
}

/**
 *  Constructor to create a stream object from a Php::Value object that holds
 *  a stream. This only works if the value contains a valid stream, otherwise
 *  an exception is thrown
 *  @param  value
 */
Stream::Stream(const Value &value) : 
    _stream(value.stream()),
    _value(value) 
{
    // it must be a valid stream
    if (!_stream) throw FatalError("Variable is not a valid stream resource");
}

/**
 *  Create a stream out of a FILE* object
 *  Note that the filedescriptor will not be closed by Stream object, you
 *  are in control, even if the stream object gets destructed, or when the
 *  close() function is explicitly called.
 *  @param  fp
 *  @param  mode
 */
Stream::Stream(FILE *fp, const char *mode) :
    _stream(open(fp, mode)),
    _value(_stream)
{
    // stream should be valid
    if (!_stream) throw FatalError("File could not be turned into a PHP stream");
}

/**
 *  Open a file, in a similar fashion as a call to fopen()
 *  See documentation for fopen() ('man fopen')
 *  @param  filename
 *  @param  mode
 */
Stream::Stream(const char *filename, const char *mode) :
    _stream(open(filename, mode)),
    _value(_stream)
{
    // stream should be valid
    if (!_stream) throw FatalError(std::string("Failed to open ") + filename + " as stream");
}

/**
 *  Create a stream with a custom implementation
 *  @param  implementation
 */
Stream::Stream(StreamImpl *implementation) :
    _stream(implementation->stream()),
    _value(_stream)
{
    // stream should be valid
    if (!_stream) throw FatalError("Invalud custom stream");
}

/**
 *  Write data to a stream
 *  @param  data
 *  @param  size
 *  @return ssize_t
 */
ssize_t Stream::write(const char *data, size_t size)
{
    // we need the tsrm_ls variable
    TSRMLS_FETCH();

    // pass on to the appropriate php function
    return _php_stream_write(_stream, (const char *)data, size TSRMLS_CC);
}

/**
 *  End of namespace
 */
}

