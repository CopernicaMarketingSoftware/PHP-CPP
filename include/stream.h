/**
 *  Stream.h
 *
 *  Class to create a PHP stream that can be used for reading and writing
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 Copernica BV
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  Forward declarations
 */
struct _php_stream;

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Stream
{
public:
    /**
     *  Constructor to create a stream object from a Php::Value object that holds
     *  a stream. This only works if the value contains a valid stream, otherwise
     *  an exception is thrown
     *  @param  value
     */
    Stream(const Value &value);
    
    /**
     *  Create a stream out of a FILE* object
     *  Note that the filedescriptor will not be closed by Stream object, you
     *  are in control, even if the stream object gets destructed, or when the
     *  close() function is explicitly called.
     *  @param  fp
     */
    Stream(FILE *fp, const char *mode);
    
    /**
     *  Open a file, in a similar fashion as a call to fopen()
     *  See documentation for fopen() ('man fopen')
     *  @param  filename
     *  @param  mode
     */
    Stream(const char *filename, const char *mode);
    
    /**
     *  Create a stream with a custom implementation
     *  This is the coolest constructor, because it allows you to create all
     *  sort of custom streams. All you have to do is supply a stream-implementation
     *  object (allocated on the heap using operator 'new'). The PHP-CPP library
     *  takes care of destructing/deleting the implementation object when it is
     *  no longer used.
     *  @param  implementation
     */
    Stream(StreamImpl *implementation);
    
    /**
     *  Destructor
     */
    virtual ~Stream() {}
    
    /**
     *  Write data to the stream
     *  @param  data        Data buffer
     *  @param  size        Size of the data
     *  @return ssize_t     Number of bytes written, or -1 on error
     */
    ssize_t write(const char *data, size_t size);
    ssize_t write(const char *data) { return write(data, ::strlen(data)); }
    ssize_t write(const std::string &data) { return write(data.c_str(), data.size()); }
    ssize_t write(const Value &data) { return data.isString() ? write(data.rawValue(), data.size()) : write(data.rawValue()); }

    /**
     *  @todo add more methods to a stream object
     */

    /**
     *  Get access to the underlying php_stream object
     *  You normally do not have to call this method, it is used by PHP-CPP internally
     *  @return php_stream
     *  @internal
     */
    struct _php_stream *stream() const { return _stream; }

private:
    /**
     *  The actual PHP stream structure
     *  @var php_stream*
     */
    struct _php_stream *_stream;

    /**
     *  The Php::Value holding the stream, we keep this as a member to ensure
     *  that the object stays in scope for as long as the stream object is active
     *  @var    Value
     */
    Value _value;
    
    /**
     *  Classes that have access to private date
     */
    friend class Value;
};

/**
 *  End of namespace
 */
}

