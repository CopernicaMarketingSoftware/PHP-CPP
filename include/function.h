/**
 *  Function.h
 * 
 *  Small extension to the Value class that allows a value to be 
 *  constructed with a std::function. 
 * 
 *  If you want to assign a std::function to a value, the following
 *  piece of code won't work:
 * 
 *      Php::Value value([]() { .... });
 * 
 *  Because the passed in function would match with many of the possible
 *  Value constructors. For that reason we have created a small and 
 *  simple Function class that can be used instead:
 * 
 *      Php::Function valu([]() { .... });
 * 
 *  A Php::Function is an extended Php::Value object, so can be used
 *  in place of Php::Values all the time. The only difference is that
 *  it has a different constructor
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
class Function : public Value
{
public:
    /**
     *  Constructor to wrap a function that takes parameters
     *  @param  function        The C++ function to be wrapped
     */
    Function(const std::function<Value(Parameters&)> &function);

    /**
     *  Constructor to wrap a function that does not accept parameters
     * 
     *  Old C++ compilers do not see a difference between std::function
     *  objects based on the function signature, so these old compilers
     *  do not see this method.
     * 
     *  @param  function        The C++ function to be wrapped
     */
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 7) || __clang__
    Function(const std::function<Value()> &function) : Function([function](Parameters &params) -> Value {
        
        // call original function, forget about the parameters
        return function();
        
    }) {}
#endif
    
    /**
     *  Destructor
     */
    virtual ~Function() {}

private:
    /**
     *  Retrieve the class entry of the _functor class
     *  @return _zend_class_entry
     */
    static struct _zend_class_entry *entry();


};

/**
 *  End namespace
 */
}

