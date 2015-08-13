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
     *  Destructor
     */
    virtual ~FatalError() throw()
    {
    }

    /**
     *  Is this a native exception (one that was thrown from C++ code)
     *  @return bool
     */
    virtual bool native() const
    {
        // although it is native, we return 0 because it should not persist
        // as exception, but it should live on as zend_error() in stead
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
