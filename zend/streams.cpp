/**
 *  Streams.cpp
 *
 *  Implementation of the streams
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"
#include <iostream>

/**
 *  Set up namespace
 */
namespace Php {

    StreamBuf::StreamBuf(int error) : _error(error)
    {
        // we reserve one byte, so that when overflow is called, we still have one
        // byte extra in the buffer to put the overflowed byte int
        setp(_buffer, _buffer + 1024 - 1);
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
 *  Some static buffers for writing data
 *  @var StreamBuf
 */
static StreamBuf bufOut        (0);
static StreamBuf bufError      (E_ERROR);
static StreamBuf bufWarning    (E_WARNING);
static StreamBuf bufNotice     (E_NOTICE);
static StreamBuf bufDeprecated (E_DEPRECATED);

/**
 *  Create the actual steams
 *  @var std::ostream
 */
std::ostream out               (&bufOut);
std::ostream error             (&bufError);
std::ostream warning           (&bufWarning);
std::ostream notice            (&bufNotice);
std::ostream deprecated        (&bufDeprecated);

/**
 *  End namespace
 */
}

