/**
 *  Functor.h
 *
 *  We want to be able to wrap a std::function in an object and pass
 *  that to PHP. The normal "Closure" class from the Zend engine
 *  would be very suitable for that. However, the Zend engine does
 *  not really allow us to add a secret pointer to such closure object.
 *
 *  Therefore, we create our own Closure class, this time using PHP-CPP
 *  code, to wrap a std::function.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Functor : public Base
{
public:
    /**
     *  Constructor
     *  @param  function    The function to wrap
     */
    Functor(const std::function<Value(Parameters &params)> &function) : _function(function) {}
    
    /**
     *  Destructor
     */
    virtual ~Functor() {}
    
    /**
     *  Invoke the functor
     *  @param  params
     *  @return Value
     */
    Value __invoke(Parameters &params) const
    {
        // pass on to the function
        return _function(params);
    }

    /**
     *  Get the functor class entry
     *  @return zend_class_entry
     */
    static zend_class_entry *entry()
    {
        // get the "member"
        return _entry;
    }

    /**
     *  Initialize the class
     *  @param  tsrmls
     */
    static void initialize(TSRMLS_D);
    
    /**
     *  Shutdown the class
     *  @param  tsrmls
     */
    static void shutdown(TSRMLS_D);

private:
    /**
     *  The std::function that is wrapped in PHP code
     *  @var std::function
     */
    const std::function<Value(Parameters &params)> _function;

    /**
     *  The classentry
     *  @var zend_class_entry
     */
    static zend_class_entry *_entry;
    
};

/**
 *  End of namespace
 */
}

