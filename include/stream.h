/**
 *  stream.h
 *
 *  Decorator class with a couple of C++ conversions for the php_stream struct
 *
 *  @author Bernardo van der Wal <bernardo@thebigb.nl>
 *  @copyright 2013, 2014
 */

struct _php_stream;
typedef struct _php_stream php_stream;

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
     *  Constructor from a php_stream struct
     *  @param  php_stream
     */
    Stream(php_stream *stream);

    /**
     *  Write the provided string to the stream
     *  Returns the number of bytes written
     *  @param  string
     *  @return size_t
     */
    virtual size_t write(std::string string);

    /**
     *  Write the provided buffer to the stream
     *  Returns the number of bytes written
     *  @param  buffer
     *  @return size_t
     */
    virtual size_t write(const char *buffer);

    /**
     *  Write the provided number of bytes of the provided buffer to the stream
     *  Returns the number of bytes written
     *  @param  buffer
     *  @param  count
     *  @return size_t
     */
    virtual size_t write(const char *buffer, size_t count);

    /**
     *  Read the provided number of bytes from the stream to the provided buffer
     *  Returns the number of bytes read
     *  @param  buffer
     *  @param  count
     *  @return size_t
     */
    virtual size_t read(char *buffer, size_t count);

    /**
     *  Read the provided number of bytes from the provided offset in the stream
     *  to the provided buffer
     *  Returns the number of bytes read
     *  @param  buffer
     *  @param  offset
     *  @param  count
     *  @return size_t
     */
    virtual size_t read(char *buffer, off_t offset, size_t count);

    /**
     *  Close the stream
     *  Returns a success or failure code
     *  @return int
     */
    virtual int close();

    /**
     *  Flush the stream buffer
     *  Returns a success or failure code
     *  @return int
     */
    virtual int flush();

    /**
     *  Seek in the stream to the provided offset relative to the provided
     *  whence
     *  Returns a success or failure code
     *  @param  offset
     *  @param  whence
     *  @return int
     */
    virtual int seek(off_t offset, int whence);

    /**
     *  Rewind the stream to the start
     *  Returns a success or failure code
     *  @return int
     */
    virtual int rewind();

    /**
     *  Returns the character at the current position of the stream
     *  @return int
     */
    virtual int getc();

    /**
     *  Write the provided character to the current position in the stream
     *  Returns a success or failure code
     *  @param  c
     *  @return int
     */
    virtual int putc(int c);

    /**
     *  Returns 1 to indicate EOF, 0 if there is no EOF and -1 to indicate an
     *  error
     *  @return int
     */
    virtual int eof();

    /**
     *  Returns a casted version of the stream as stdio FILE pointer
     *  @return FILE *
     */
    virtual FILE *stdioFile() const;

    /**
     *  Cast to a stdio FILE pointer
     *  @return FILE *
     */
    operator FILE * () const
    {
        return stdioFile();
    }

protected:

    /**
     *  Internal reference to the php_stream struct
     */
    php_stream *_stream;
};

}
