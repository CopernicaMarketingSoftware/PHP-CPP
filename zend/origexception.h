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

    /**
     *  The PHP exception code
     *  @var    long int
     */
    long int _code;

    /**
     *  PHP source file
     *  @var    std::string
     */
    std::string _file;

    /**
     *  PHP source line
     *  @var    long int
     */
    long int _line;

public:
    /**
     *  Constructor
     *  @param  object  The object that was thrown
     */
    OrigException(zend_object *object) : Exception(std::string{ ZSTR_VAL(object->ce->name), ZSTR_LEN(object->ce->name) })
    {
        // the result value from zend and the object zval
        zval result, properties;

        // retrieve the object properties
        ZVAL_OBJ(&properties, object);

        // retrieve the message, filename, error code and line number
        auto message = zval_get_string(zend_read_property(Z_OBJCE(properties), &properties, ZEND_STRL("message"), 1, &result));
        auto file    = zval_get_string(zend_read_property(Z_OBJCE(properties), &properties, ZEND_STRL("file"   ), 1, &result));
        auto code    = zval_get_long  (zend_read_property(Z_OBJCE(properties), &properties, ZEND_STRL("code"   ), 1, &result));
        auto line    = zval_get_long  (zend_read_property(Z_OBJCE(properties), &properties, ZEND_STRL("line"   ), 1, &result));

        // store the message, code, filename and line number
        _message.assign(ZSTR_VAL(message), ZSTR_LEN(message));
        _code = code;
        _file.assign(ZSTR_VAL(file), ZSTR_LEN(file));
        _line = line;

        // clean up message and file strings
        zend_string_release(message);
        zend_string_release(file);
    }

    /**
     *  Copy constructor
     *  @param  exception
     */
    OrigException(const OrigException &exception) :
        Exception("OrigException"), _handled(exception._handled) {}

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
    virtual long int code() const _NOEXCEPT override
    {
        // return the stored code
        return _code;
    }

    /**
     *  Retrieve the filename the exception was thrown in
     *
     *  @return The filename the exception was thrown in
     */
    virtual const std::string& file() const _NOEXCEPT override
    {
        // return the stored filename
        return _file;
    }

    /**
     *  Retrieve the line at which the exception was thrown
     *
     *  @return The line number the exception was thrown at
     */
    virtual long int line() const _NOEXCEPT override
    {
        // return the stored line number
        return _line;
    }
};

/**
 *  Global function to process an exception
 *  @param  exception
 */
static inline void process(Exception &exception)
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
