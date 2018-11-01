/**
 *  stream.cpp
 *
 *  Implementation for the Stream class, which wraps a php_stream struct into a
 *  C++ object with a couple of helper methods
 *
 *  @author Bernardo van der Wal <bernardo@thebigb.nl>
 *  @copyright 2015 Bernardo van der Wal
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Constructor from a php_stream struct
 *  @param  php_stream
 */
Stream::Stream(php_stream *stream)
{
    if (_stream == NULL)
    {
        throw Php::Exception("Could not initialize stream object. The resource is invalid.");
    }
    _stream = stream;
}

/**
 *  Write the provided string to the stream
 *  Returns the number of bytes written
 *  @param  string
 *  @return size_t
 */
size_t Stream::write(std::string string)
{
    return php_stream_write(_stream, string.c_str(), string.length());
}

/**
 *  Write the provided buffer to the stream
 *  Returns the number of bytes written
 *  @param  buffer
 *  @return size_t
 */
size_t Stream::write(const char *buffer)
{
    return php_stream_write(_stream, buffer, std::strlen(buffer));
}

/**
 *  Write the provided number of bytes of the provided buffer to the stream
 *  Returns the number of bytes written
 *  @param  buffer
 *  @param  count
 *  @return size_t
 */
size_t Stream::write(const char *buffer, size_t count)
{
    return php_stream_write(_stream, buffer, count);
}

/**
 *  Read the provided number of bytes from the stream to the provided buffer
 *  Returns the number of bytes read
 *  @param  buffer
 *  @param  count
 *  @return size_t
 */
size_t Stream::read(char *buffer, size_t count)
{
    return php_stream_read(_stream, buffer, count);
}

/**
 *  Read the provided number of bytes from the provided offset in the stream
 *  to the provided buffer
 *  Returns the number of bytes read
 *  @param  buffer
 *  @param  offset
 *  @param  count
 *  @return size_t
 */
size_t Stream::read(char *buffer, off_t offset, size_t count)
{
    if (php_stream_seek(_stream, offset, SEEK_SET) == 0)
    {
        return php_stream_read(_stream, buffer, count);
    }
    return 0;
}

/**
 *  Close the stream
 *  Returns a success or failure code
 *  @return int
 */
int Stream::close()
{
    if (_stream->is_persistent == 0)
    {
        return php_stream_close(_stream);
    }
    else
    {
        return php_stream_pclose(_stream);
    }
}

/**
 *  Flush the stream buffer
 *  Returns a success or failure code
 *  @return int
 */
int Stream::flush()
{
    return php_stream_flush(_stream);
}

/**
 *  Seek in the stream to the provided offset relative to the provided
 *  whence
 *  Returns a success or failure code
 *  @param  offset
 *  @param  whence
 *  @return int
 */
int Stream::seek(off_t offset, int whence)
{
    return php_stream_seek(_stream, offset, whence);
}

/**
 *  Rewind the stream to the start
 *  Returns a success or failure code
 *  @return int
 */
int Stream::rewind()
{
    return php_stream_rewind(_stream);
}

/**
 *  Returns the character at the current position of the stream
 *  @return int
 */
int Stream::getc()
{
    return php_stream_getc(_stream);
}

/**
 *  Write the provided character to the current position in the stream
 *  Returns a success or failure code
 *  @param  c
 *  @return int
 */
int Stream::putc(int c)
{
    return php_stream_putc(_stream, c);
}

/**
 *  Returns 1 to indicate EOF, 0 if there is no EOF and -1 to indicate an
 *  error
 *  @return int
 */

int Stream::eof()
{
    return php_stream_eof(_stream);
}

/**
 *  Returns a casted version of the stream as stdio FILE pointer
 *  @return FILE *
 */
FILE *Stream::stdioFile() const
{
    FILE* file = NULL;
    if(php_stream_can_cast(_stream, PHP_STREAM_AS_STDIO) == SUCCESS)
    {
        php_stream_cast(_stream, PHP_STREAM_AS_STDIO, (void**)&file, 0);
    }
    return file;
}

}
