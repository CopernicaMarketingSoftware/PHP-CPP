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
 
