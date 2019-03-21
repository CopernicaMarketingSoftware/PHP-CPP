/**
 *  State.h
 * 
 *  Class that is used to store the state of current callstack and
 *  whether an exception is now active. We use this before we make
 *  a call to PHP userspace to see if the state has changed afterwards,
 *  which means that an exception has bubbled up that should be handled
 *  first.
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2019 Copernica BV
 */

/**
 *  Begin of namespace
 */
namespace Php {

/**
 *  Class definition
 */
class State
{
private:
    /**
     *  The current exception that was active when state was registered
     *  @var zend_object
     */
    zend_object *_exception;

    /**
     *  Helper method to check if a zend-class is an instance of a certain class
     *  @param  entry       the class entry to check
     *  @param  required    the required class entry
     *  @return bool
     */
    static bool instanceof(const zend_class_entry *entry, const zend_class_entry *required)
    {
        // check the class name
        if (entry == required) return true;
        
        // is there a base class?
        if (entry->parent == nullptr) return false;
        
        // check the parent
        return instanceof(entry->parent, required);
    }
    
    /**
     *  Helper method to check if a zend-object is an instance of a certain class
     *  @param  object      the object to check
     *  @param  required    required class entry
     *  @return bool
     */
    static bool instanceof(const zend_object *object, zend_class_entry *required)
    {
        return instanceof(object->ce, required);
    }
    
    
public:
    /**
     *  Constructor
     */
    State() : _exception(EG(exception)) {}
    
    /**
     *  Destructor
     */
    virtual ~State() = default;
    
    /**
     *  Rethrow the exception so that it ends up in the extension
     * 
     *  When this method returns, it means that no exception occured,
     *  and the call to PHP space was a success. Otherwise it throws
     *  an exception. Note that this is not exactly in line with our
     *  coding convention that says that methods are not supposed to
     *  throw exceptions.
     * 
     *  @throw Throwable
     */
    void rethrow()
    {
        // is an exception now active
        zend_object *current = EG(exception);
        
        // if no exception is active
        if (current == nullptr) return;
        
        // or if the exception did not change
        if (current == _exception) return;
        
        // an exception occured, this can be a PHP error or a PHP exception
        if (instanceof(current, zend_ce_error)) throw RethrowableError(current);
        
        // otherwise we wrap the exception
        throw RethrowableException(current);
    }
};
    
/**
 *  End of namespace
 */
}

