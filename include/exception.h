/**
 *  Exception.h
 *  Implementation of Php Exceptions.
 *
 *  @author Jasper van Eck <jasper.vaneck@copernica.com>
 *  @copyright 2013, 2014 Copernica BV
 */
#include <exception>
#include <string>

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT Exception : public std::exception
{
protected:
    /**
     *  The exception message
     *  @var    char*
     */
    std::string _message;

public:
    /**
     *  Constructor
     *
     *  @param  message The exception message
     */
    Exception(std::string message) : _message(std::move(message)) {}

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
     *
     *  @note   This only works if the exception was originally
     *          thrown in PHP userland. If the native() member
     *          function returns true, this function will not
     *          be able to correctly provide the filename.
     *
     * @return The exception code
     */
    virtual long int code() const _NOEXCEPT
    {
        return -1;
    }

    /**
     *  Retrieve the filename the exception was thrown in
     *
     *  @note   This only works if the exception was originally
     *          thrown in PHP userland. If the native() member
     *          function returns true, this function will not
     *          be able to correctly provide the filename.
     *
     *  @return The filename the exception was thrown in
     */
    virtual const std::string& file() const _NOEXCEPT
    {
        // we don't know the file the exception is from
        static std::string file{ "<filename unknown>" };

        // return the missing filename
        return file;
    }

    /**
     *  Retrieve the line at which the exception was thrown
     *
     *  @note   This only works if the exception was originally
     *          thrown in PHP userland. If the native() member
     *          function returns true, this function will not
     *          be able to correctly provide the line number.
     *
     *  @return The line number the exception was thrown at
     */
    virtual long int line() const _NOEXCEPT
    {
        // we don't know the file the exception is from
        return -1;
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
};

/**
 *  End of namespace
 */
}
