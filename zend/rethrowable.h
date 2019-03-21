/**
 *  Rethrowable.h
 *
 *  Class that we use to wrap user space exceptions in, so that we can
 *  monitor whether they were handled by the extension or not.
 *
 *  How does this work? When the C++ extension makes a call to PHP 
 *  user-space, it is possible that this call throws an error or an
 *  exception:
 *
 *      // make a call to the user-space function
 *      Php::call("my_userspace_function");
 *
 *  There are two options: the extension then catches this exception, or 
 *  the exception is not caught by the extension. If the exception is
 *  handled by the extension we should tell the zend-engine that the
 *  exception was handled, otherwise we tell nothing to the zend-engine
 *  (which causes the exception to bubble up).
 *
 *  This Rethrowable class is used to wrap the exceptions. It is supposed 
 *  to end up in the extension. If the extension catches it, it is nicely 
 *  destructed. But if it is not caught by the extension, it will end up 
 *  in PHP-CPP code, where we can tell that it should not nicely destruct, 
 *  but end up in PHP space anyway.
 *  
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2019 Copernica BV
 */

/**
 *  Begin of namespace
 */
namespace Php {

/**
 *  Base class for WrappedException and WrappedError
 */
class Rethrowable
{
protected:
    /**
     *  Should the object be rethrown?
     *  We assume this to be false, if the object ends up in PHP-CPP space
     *  again (and the exception did not catch it) we reset this property.
     *  @var bool
     */
    bool _rethrow = false;
    
protected:
    /**
     *  Protected constructor (only ThrowableException and ThrowableError should be extended)
     */
    Rethrowable() {}

public:
    /**
     *  Copy constructor
     *  @param  that
     */
    Rethrowable(const Rethrowable &that) : _rethrow(that._rethrow) {}
    
    /**
     *  Move constructor
     *  @param  that
     */
    Rethrowable(Rethrowable &&that) : _rethrow(that._rethrow)
    {
        // reset other object
        that._rethrow = false;
    }
    
    /**
     *  Destructor
     */
    virtual ~Rethrowable()
    {
        // if we have to rethrow, we keep it on the stack
        if (_rethrow) return;
        
        // tell the zend engine to remove it from the stack
        zend_clear_exception();
    }
};

/**
 *  Wrapper around the Php::Exception class
 */
class RethrowableException : public Exception, public Rethrowable
{
public:
    /**
     *  Constructor
     *  @param  object
     */
    RethrowableException(zend_object *object) : Exception(object) {}
    
    /**
     *  Destructor
     */
    virtual ~RethrowableException() = default;

    /**
     *  Rethrow the exception
     */
    virtual void rethrow() override
    {
        // update member
        _rethrow = true;
    }
};

/**
 *  Wrapper around the Php::Error class
 */
class RethrowableError : public Error, public Rethrowable 
{
public:
    /**
     *  Constructor
     *  @param  object
     */
    RethrowableError(zend_object *object) : Error(object) {}
    
    /**
     *  Destructor
     */
    virtual ~RethrowableError() = default;

    /**
     *  Rethrow the exception
     */
    virtual void rethrow() override
    {
        // update member
        _rethrow = true;
    }
};

/**
 *  End of namespace
 */
}


 
