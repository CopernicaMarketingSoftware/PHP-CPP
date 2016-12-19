/**
 *  OrigException.h
 *
 *  Class that wraps around an exception that was thrown by PHP code,
 *  and that could - but not necessarily has to - be caught by C++
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class OrigException : public Exception
{
private:
    /**
     *  Is this a an exception that was caught by extension C++ code.
     *
     *  When the object is initially created, we assume that it will be caught
     *  by C++ code. If it later turns out that the PHP-CPP can catch this
     *  exception after the extension C++ code ran, the variable is set back
     *  to false.
     *
     *  @var bool
     */
    bool _handled = true;

public:
    /**
     *  Constructor
     *  @param  object  The object that was thrown
     */
    OrigException(zend_object *object) : Exception(std::string{ ZSTR_VAL(object->ce->name), ZSTR_LEN(object->ce->name) })
    {
    }

    /**
     *  Copy constructor
     *  @param  exception
     */
    OrigException(const OrigException &exception) :
        Exception("OrigException"), _handled(exception._handled)
    {
    }

    /**
     *  Move constructor
     *  @param  exception
     */
    OrigException(OrigException &&exception) :
        Exception("OrigException"), _handled(exception._handled)
    {
        // set other exception to handled so that it wont do anything on destruction
        exception._handled = true;
    }

    /**
     *  Destructor
     */
    virtual ~OrigException() throw()
    {
        // if the exception was not handled by C++ code, we're not going to do anything
        // and the exception stays active
        if (!_handled) return;

        // the exception was handled, so we should clean it up
        zend_clear_exception();
    }

    /**
     *  This is _not_ a native exception, it was thrown by a PHP script
     *  @return bool
     */
    virtual bool native() const override
    {
        return false;
    }

    /**
     *  Reactivate the exception
     */
    void reactivate()
    {
        // it was not handled by extension C++ code
        _handled = false;
    }
};

/**
 *  Global function to process an exception
 *  @param  exception
 */
inline void process(Exception &exception)
{
    // is this a native exception?
    if (exception.native())
    {
        // the exception is native, call the zend throw method
        zend_throw_exception(zend_exception_get_default(), (char *)exception.what(), 0);
    }

    // or does it have its own report function?
    else if (!exception.report())
    {
        // this is not a native exception, so it was originally thrown by a
        // php script, and then not caught by the c++ of the extension, we are
        // going to tell to the exception that it is still active
        OrigException &orig = static_cast<OrigException&>(exception);

        // reactive the exception
        orig.reactivate();
    }
}

/**
 *  End of namespace
 */
}
