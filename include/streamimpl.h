/**
 *  StreamImpl.h
 *
 *  If you want to create your own streams and add them to the PHP engine,
 *  you can do so by extending the StreamImpl class, and implementing all
 *  appropriate methods. The streamimpl instance should be passed to a stream
 *  constructor
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 Copernica BV
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  Include guard
 */
namespace Php {

/**
 *  Class definition
 */
class StreamImpl
{
public:
    /**
     *  Constructor
     *  @param  label           Stream label (useful for debugging / showed in error messages)
     */
    StreamImpl(const char *label = "PHP-CPP custom stream");
    
    /**
     *  Destructor
     */
    virtual ~StreamImpl();
    
    /**
     *  Retrieve the underlying stream
     *  @return struct _php_stream
     *  @internal
     */
    struct _php_stream *stream() const
    {
        return _stream;
    }
    
    /**
     *  Write data to the stream
     *  Returns the number of bytes written.
     *  @param  data
     *  @param  size
     *  @return size_t
     */
    virtual size_t write(const char *data, size_t size) = 0;
    
    /**
     *  Read data from a stream
     *  @param  data
     *  @param  size
     *  @return size_t
     */
    virtual size_t read(char *data, size_t size) = 0;
    
    /**
     *  Flush the stream
     *  @return int
     */
    virtual int flush() = 0;
    
    /**
     *  Close the stream
     *  @param  close_handle
     *  @return int
     */
    virtual int close(int close_handle) = 0;

private:
    /**
     *  Structure holding all stream operations
     *  (Dynamically allocated because we do not want to expose the size
     *  of the structure to outside PHP-CPP, so that client applications can
     *  use PHP-CPP without having to include any of the Zend headers)
     *  @var    php_stream_ops
     */
    struct _php_stream_ops *_operations;

    /**
     *  The actual php stream
     *  @var    php_stream
     */
    struct _php_stream *_stream;

};
    
/**
 *  End of namespace
 */
}

