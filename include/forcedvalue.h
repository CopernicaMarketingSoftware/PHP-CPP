/**
 *  ForcedValue.h
 *
 *  The ForcedValue is a wrapper around the value class that ensures that a 
 *  certain property always has a certain type.
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
template <Type TYPE> 
class ForcedValue : public Value
{
public:
    /**
     *  Constructor
     */
    ForcedValue() : Value() { setType(TYPE); }
    
    /**
     *  Copy constructor
     *  @param  that
     */
    ForcedValue(const ForcedValue<TYPE> &that) : Value(that) {}
    
    /**
     *  Move constructor
     *  @param  that
     */
    ForcedValue(ForcedValue<TYPE> &&that) : Value(std::move(that)) {}
    
    /**
     *  Copy constructor from a value object
     *  @param  value
     */
    ForcedValue(const Value &value) : Value(value) { setType(TYPE); }
    
    /**
     *  Wrap object around zval
     *  @param  zval        Zval to wrap
     *  @param  ref         Force this to be a reference
     */
    ForcedValue(struct _zval_struct *zval, bool ref = false) : Value(zval, ref) { setType(TYPE); }
    
    /**
     *  Destructor
     */
    virtual ~ForcedValue() {}

    /**
     *  Change the internal type of the variable
     *  @param  Type
     */
    virtual Value &setType(Type type) override
    {
        // call base
        return Value::setType(TYPE);
    }
    
protected:
    /**
     *  Validate the object
     *  @return Value
     */
    virtual Value &validate() override
    {
        // make sure the object has a valid type
        setType(TYPE);
        
        // call base
        return Value::validate();
    }
    
};

/**
 *  Define for arrays and objects
 */
using Array     =   ForcedValue<Type::Array>;
using Object    =   ForcedValue<Type::Object>;
    
/**
 *  End of namespace
 */
}

