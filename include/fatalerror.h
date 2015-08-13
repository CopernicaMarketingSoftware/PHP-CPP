/**
 *  @file fatalerror.h
 *
 *  This file provides a representation of a fatal error that occured within the
 *  Php environment which is mapped to a call of zend_error()
 *
 *  Normally, fatal errors are reported with a call to zend_error().
 *
 *  However, this will trigger a longjmp(), which will cause objects
 *  constructed in the extension not to be destructed. Therefore we use 
 *  this FatalError class, which is a normal exception that _does_
 *  cause objects to be destructed.
 *
 *  When it is caught, right before control is handed back to the Zend
 *  engine, it will turn the exception into a zend_error() call and
 *  thus a longjmp.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *
 *  @copyright 2014-2015 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT FatalError : public Exception
{
public:
    /**
     *  Constructor to create an object of this type by specifying the following
     *
     *  @param message - The message to convey
     */
    explicit FatalError(const std::string & message)
        : Exception { message }
    {}

    /**
     *  Default destructor
     */
    virtual ~FatalError() _NOEXCEPT =default;

    /**
     *  Method stating whether this is native exception
     *  (one that was thrown from C++ code)
     *
     *  @return bool - true if the exception is native, false otherwise
     */
    virtual bool native() const _NOEXCEPT override
    {
        // although it is native, we return false because it should not persist
        // as an exception but should live on as zend_error() instead
        return false;
    }

    /**
     *  Report this error as a fatal error
     *  @return bool
     */
    virtual bool report() const override;
};

/**
 *  End of namespace
 */
}
