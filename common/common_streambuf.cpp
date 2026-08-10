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
 *  End namespace
 */
}
 