/**
 *  Array.h
 *
 *  An array is an extension to the Value class. It extends the Value class
 *  to initialize the variable as an array, instead of a null pointer
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
class Array : public Value
{
public:
    /**
     *  Constructor
     */
    Array() : Value() { setType(arrayType); }
    
    /**
     *  Copy constructor
     *  @param  array
     */
    Array(const Array &array) : Value(array) {}
    
    /**
     *  Move constructor
     *  @param  array
     */
    Array(Array &&that) : Value(std::move(that)) {}
    
    /**
     *  Copy constructor from a value object
     *  @param  value
     */
    Array(const Value &value) : Value(value) { setType(arrayType); }
    
    /**
     *  Destructor
     */
    virtual ~Array() {}

    /**
     *  Change the internal type of the variable
     *  @param  Type
     */
    virtual Value &setType(Type type) override
    {
        // only possible for arrays
        if (type != arrayType) return *this;
        
        // call base
        return Value::setType(type);
    }
    
protected:
    /**
     *  Validate the object
     *  @return Value
     */
    virtual Value &validate() override
    {
        // make sure the value object is an array
        setType(arrayType);
        
        // call base
        return Value::validate();
    }
    
};
    
/**
 *  End of namespace
 */
}

