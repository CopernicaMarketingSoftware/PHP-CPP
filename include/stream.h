/**
 *  Stream.h
 * 
 *  PHP internally uses a "stream" concept. This is a complicated
 *  abstraction layer for sockets and filedescriptors and other sources
 *  from which data can be read and/or to which data can be sent.
 * 
 *  This Php::Stream class can be used to wrap around a value (if that
 *  value contains a stream) to access stream-specific properties. This
 *  class currently has a very limited API, but more features can be
 *  added if needed.
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2019 Copernica BV
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  Forward declaration
 */
struct _php_stream;

/**
 *  Begin of namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT Stream
{
private:
    /**
     *  The wrapped value that is suppoed to hold a stream
     *  @var Value
     */
    Value _value;
    
    /**
     *  Object holding the stream data
     *  @var struct _php_stream
     */
    struct _php_stream *_stream;

public:
    /**
     *  Constructor
     *  @param  value       value to be wrapped
     *  @throws std::runtime_error
     */
    Stream(const Value &value);
    
    /**
     *  No copying
     *  @param  that
     */
    Stream(const Stream &that) = delete;
    
    /**
     *  Destructor
     */
    virtual ~Stream();
    
    /**
     *  Size of the read-buffer (number of bytes that have already been read
     *  from the underlying filesystem, but that are not yet passed to php space)
     *  @return size_t
     */
    size_t readbuffer() const;
    
    /**
     *  Get access to the internal filedescriptor
     *  Note that not every stream is always associated with a filedescriptor,
     *  in which case this function returns -1
     *  @return int
     */
    int fd() const;
};
    
/**
 *  End of namespace
 */
}

