/**
 *  StreamImpl.cpp
 *
 *  Implementation file for the StreamImpl class
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
 *  Function that is called to write to a custom stream
 *  @param  stream
 *  @param  buffer
 *  @param  size
 *  @return size_t
 */
static size_t custom_write(php_stream *stream, const char *buffer, size_t size TSRMLS_DC)
{
    // retrieve the implementation
    StreamImpl *impl = (StreamImpl *)stream->abstract;

    // @todo
    //  this function returns a size_t, but how does php see the difference
    //  between an error, and writing 0 bytes??

    // call implementation
    return impl->write(buffer, size);
}

/**
 *  Function that gets called when reading from a custom stream
 *  @param  stream
 *  @param  buffer
 *  @param  size
 *  @return size_t
 */
static size_t custom_read(php_stream *stream, char *buffer, size_t size TSRMLS_DC)
{
    // retrieve the implementation
    StreamImpl *impl = (StreamImpl *)stream->abstract;

    // @todo
    //  this function returns a size_t, but how does php see the difference
    //  between an error, and reading 0 bytes??

    // call implementation
    return impl->read(buffer, size);
}

/**
 *  Function that gets called to close a custom stream
 *  @param  stream
 *  @param  close_handle
 *  @return int
 */
static int custom_close(php_stream *stream, int close_handle TSRMLS_DC)
{
    // @todo what does this close_handle do?
    
    // retrieve the implementation
    StreamImpl *impl = (StreamImpl *)stream->abstract;

    // call implementation
    // @todo what is the point of passing the close handle to the implementation object
    int result = impl->close(close_handle);
    
    // @todo what to do with the return value?
    
    // the close call is also the last call to the stream, so we can just as 
    // well destroy it (php is so nice that it will always call this close
    // function for us)
    delete impl;
    
    // done
    return result;
}

/**
 *  Function that gets called to flush a custom stream
 *  @param  stream
 *  @return int
 */
static int custom_flush(php_stream *stream TSRMLS_DC)
{
    // retrieve the implementation
    StreamImpl *impl = (StreamImpl *)stream->abstract;
    
    // call implementation
    return impl->flush();
}

/**
 *  Constructor
 *  @param  label           Stream label (useful for debugging / showed in error messages)
 */
StreamImpl::StreamImpl(const char *label)
{
    // allocate the operations
    _operations = new php_stream_ops;
    
    // set all members
    _operations->write = custom_write;
    _operations->read = custom_read;
    _operations->close = custom_close;
    _operations->flush = custom_flush;
    _operations->label = label;
    
    // features that are not yet implemented
    _operations->seek = nullptr;
    _operations->cast = nullptr;
    _operations->stat = nullptr;
    _operations->set_option = nullptr;
    
    // allocate the stream
    // @todo 
    //  what mode should we pass in - and why do we on earth do we have to pass
    //  in a r/w mode? we're making a custom stream, why would that be restricted
    //  to the modes that stdio FILE's have?????
    _stream = _php_stream_alloc(_operations, this, nullptr, "r+b" TSRMLS_CC);
}

/**
 *  Destructor
 */
StreamImpl::~StreamImpl()
{
    // we no longer need the operations structure
    delete _operations;
}

/**
 *  End of namespace
 */
}

