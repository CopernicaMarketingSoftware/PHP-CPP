/**
 *  Stream.cpp
 * 
 *  Implementation file for the stream class.
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2019 Copernica BV
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Begin of namespace
 */
namespace Php {

/**
 *  Constructor
 *  @param  value       value to be wrapped
 *  @throws std::runtime_error
 */
Stream::Stream(const Value &value) : _value(value)
{
    // get the stream object
    php_stream_from_zval_no_verify(_stream, _value._val);
    
    // was this indeed a stream?
    if (_stream == nullptr) throw std::runtime_error("variable does not hold a stream");
}

/**
 *  Destructor
 */
Stream::~Stream() {}

/**
 *  Size of the read-buffer (number of bytes that have already been read
 *  from the underlying filesystem, but that are not yet passed to php space)
 *  @return size_t
 */
size_t Stream::readbuffer() const
{
    // do the math
    return _stream->writepos - _stream->readpos;
}

/**
 *  Get access to the internal filedescriptor
 *  Note that not every stream is always associated with a filedescriptor,
 *  in which case this function returns -1
 *  @return int
 */
int Stream::fd() const
{
    // the return value
    int retval = -1;
    
    // cast the stream to a filedescriptor, the cast-internal parameter is supposed to be to get
    // rid of warnings about buffered data (this code was copied from streamfuncs.c). We use
    // "FOR_SELECT" casting, as that is the most simple operations (without "FOR_SELECT" it flushes too)
    auto result = php_stream_cast(_stream, PHP_STREAM_AS_FD_FOR_SELECT | PHP_STREAM_CAST_INTERNAL, (void **)&retval, 0);
    
    // on failure we return -1
    return result == SUCCESS ? retval : -1;
}
    
/**
 *  End of namespace
 */
}

