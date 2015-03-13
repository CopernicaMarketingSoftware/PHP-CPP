/**
 *  StreamBuf.h
 *
 *  PHP output stream buffer which is used by the Php::out object to
 *  have an output stream just like the regular std::ostream buffers, 
 *  but that sends all output to PHP output
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Issue reported by user on a Windows / Mingw32 platform: EOF was
 *  not defined. Let's defined it ourselves
 */
#ifndef EOF
# define EOF (-1)
#endif

/**
 *  Class definition
 */
class StreamBuf : public std::streambuf
{
public:
    /**
     *  Constructor
     *  @param  error   the error type, or 0 for regular output
     */
    StreamBuf(int error);
    
    /**
     *  No copying or moving
     *  @param  that
     */
    StreamBuf(const StreamBuf &that) = delete;
    StreamBuf(StreamBuf &&that) = delete;
    
    /**
     *  Destructor
     */
    virtual ~StreamBuf() {}
    
    /**
     *  No copying or moving
     *  @param  that
     */
    StreamBuf &operator=(const StreamBuf &that) = delete;
    StreamBuf &operator=(StreamBuf &&that) = delete;

protected:
    /**
     *  Method that is called when the internal buffer overflows
     *  @param  c
     *  @return int
     */
    virtual int overflow(int c = EOF) override;
    
    /**
     *  Called when the internal buffer should be synchronized
     *  @return int
     */
    virtual int sync() override;

private:
    /**
     *  The error type, or 0 for regular output
     *  @var    int
     */
    int _error;

    /**
     *  The internal buffer
     *  @var    char[]
     */
    char _buffer[1024];
};

/**
 *  End namespace
 */
}

