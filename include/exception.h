/**
 *  Exception.h
 *  Implementation of Php Exceptions.
 *
 *  @author Jasper van Eck <jasper.vaneck@copernica.com>
 *  @copyright 2013, 2014 Copernica BV
 */
#include <exception>

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT Exception : public std::exception
{
private:
    /**
     *  The exception message
     *  @var    char*
     */
    std::string _message;

    /**
     *  The PHP exception code
     *  @var    long int
     */
    long int _code;

    /**
     * PHP source file
     */
    std::string _file;

    /**
     * PHP source line
     */
    long int _line;

    /**
     *  Has this exception been processed by native C++ code?
     *  @var    bool
     */
    bool _processed = false;

public:
    /**
     *  Constructor
     *  @param  &string
     */
    Exception(std::string message, long int code = 0) : std::exception(), _message(std::move(message)), _code(code) {}

    /**
     *  Destructor
     */
    virtual ~Exception() = default;

    /**
     *  Overridden what method
     *  @return const char *
     */
    virtual const char *what() const _NOEXCEPT override
    {
        return _message.c_str();
    }

    /**
     *  Returns the message of the exception.
     *  @return &string
     */
    const std::string &message() const throw()
    {
        return _message;
    }

    /**
     * Returns the exception code
     * @return long int
     */
    long int code() const _NOEXCEPT
    {
        return _code;
    }

    const std::string& file() const _NOEXCEPT
    {
        return _file;
    }

    long int line() const _NOEXCEPT
    {
        return _line;
    }

    /**
     *  Is this a native exception (one that was thrown from C++ code)
     *  @return bool
     */
    virtual bool native() const
    {
        // yes, it is native
        return true;
    }

    /**
     *  Report this error as a fatal error
     *  @return bool
     */
    virtual bool report() const
    {
        // this is not done here
        return false;
    }

protected:
    /**
     * Set the message of the exception
     * @param msg
     */
    void setMessage(const std::string& msg)
    {
        _message = msg;
    }

    /**
     * Set the exception code
     * @param code
     */
    void setCode(long int code)
    {
        _code = code;
    }

    /**
     * Set the source file
     * @param file
     */
    void setFile(const std::string& file)
    {
        _file = file;
    }

    /**
     * Set the source line
     * @param line
     */
    void setLine(long int line)
    {
        _line = line;
    }
};

/**
 *  End of namespace
 */
}
