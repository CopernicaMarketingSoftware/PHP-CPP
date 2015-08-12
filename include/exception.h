/**
 *  @file exception.h
 *
 *  This file provides a class to represent Php Exceptions
 *
 *  @author Jasper van Eck <jasper.vaneck@copernica.com>
 *
 *  @copyright 2013-2015 Copernica BV
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
public:
    /**
     *  Constructor to create an object of this type by specifying the following
     *
     *  @param message - The message to convey
     *  @param code    - A Php exception code
     */
    explicit Exception(const std::string & message, int code = 0)
        : std::exception {         }
        , _message       { message }
        , _code          { code    }
    {}

    /**
     *  Destructor
     */
    virtual ~Exception() throw() {}

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

private:
    /**
     *  The exception message
     */
    std::string _message;

    /**
     *  The PHP exception code
     */
    int _code;

    /**
     *  State field tracking whether this exception has been
     *  processed by native C++ code
     */
    bool _processed { false };
};

/**
 *  End of namespace
 */
}
