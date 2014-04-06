/**
 *  StreamBuf.cpp
 *
 *  Implementation file for the StreamBuf class
 *
 *  @see http://www.mr-edd.co.uk/blog/beginners_guide_streambuf
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Constructor
 *  @param  error
 */
StreamBuf::StreamBuf(int error) : _error(error)
{
    // we reserve one byte, so that when overflow is called, we still have one
    // byte extra in the buffer to put the overflowed byte int
    setp(_buffer, _buffer+1024-1);
}
    

/**
 *  Method that is called when the internal buffer overflows
 *  @param  c
 *  @return int
 */
int StreamBuf::overflow(int c)
{
    // for error buffers, overflow is simply discarded
    if (_error) return c;
    
    // end-of-file has not output, we call EOF directly, and by using the
    // comma operator we ensure that EOF is returned
    if (c == EOF) return sync(), EOF;
    
    // because we lied the underlying buffer about the size of the buffer
    // by one byte, there is no real overflow, and we can still add the byte
    // to the end of the buffer
    *pptr() = c;
    
    // increment buffer size
    pbump(1);
    
    // and now we're going to syn the buffer
    return sync() == -1 ? EOF : c;
}

/**
 *  Called when the internal buffer should be synchronized
 *  @return int
 */
int StreamBuf::sync()
{
    // current buffer size
    size_t size = pptr() - pbase();
    
    // is this the error stream or the regular output stream?
    if (_error)
    {
        // write to error (the zend_error() method is a varargs function, 
        // which means that we have to include a printf() like format as first
        // parameter. We can not specify pbase() directly, because (1) it is
        // not null terminated and (2) it could contain % signs and allow all
        // sorts of buffer overflows.
        zend_error(_error, "%.*s", (int)size, pbase());
        
    }
    else
    {
        // write to zend
        zend_write(pbase(), size);
    }
    
    // reset the buffer
    pbump(-size);
    
    // done
    return 0;
}

/**
 *  End namespace
 */
}
 