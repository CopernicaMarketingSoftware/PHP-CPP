/**
 *  @file exception.h
 *
 *  This file provides a class to represent Php Exceptions.
 *
 *  @author Jasper van Eck <jasper.vaneck@copernica.com>
 *  @copyright 2013, 2014 Copernica BV
 */

 /**
  * Platform dependency
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
    explicit Exception(const std::string &message, int code = 0)
        : std::exception{         }
        , m_message     { message }
        , m_code        { code    }
    {}

    /**
     *  Default destructor
     */
    virtual ~Exception() _NOEXCEPT =default;

    /**
     *  Overriden method to retrieve the exception message
     *
     *  @return const char * - C-String containing the message associated
     *                         with this exception
     */
    virtual const char *what() const _NOEXCEPT override
    {
        return m_message.c_str();
    }

    /**
     *  Method to retrieve the exception message
     *
     *  @return const string & - std::string object containing the message
     *                           associated with this exception
     */
    const std::string &message() const _NOEXCEPT
    {
        return m_message;
    }

    /**
     *  Method stating whether this is native exception
     *  (one that was thrown from C++ code)
     *
     *  @return bool - true if the exception is native, false otherwise
     */
    virtual bool native() const _NOEXCEPT
    {
        // yes, its a native exception by default
        return true;
    }

    /**
     *  Method stating whether this is a fatal error
     *
     *  @return bool - true if its a fatal error, false otherwise
     */
    virtual bool report() const _NOEXCEPT
    {
        // no, it's not a fatal error by default
        return false;
    }

private:
    /**
     *  The exception message
     */
    std::string m_message;

    /**
     *  The PHP exception code
     */
    int m_code;

    /**
     *  State field tracking whether this exception has been
     *  processed by native C++ code?
     */
    bool m_processed{ false };
};

/**
 *  End of namespace
 */
}
