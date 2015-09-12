/**
 *  @file arithmethic.h
 *
 *  This file defines a class that takes care of arithmetic operations on
 *  PHP variables
 *
 *  @author Emiel Bruijntjes   <emiel.bruijntjes@copernica.com>
 *  @author Rico Antonio Felix <ricoantoniofelix@yahoo.com>
 *
 *  @copyright 2013-2015 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {
    
/**
 *  Class definition
 */
template < template <typename T> class F>
class Arithmetic
{
public:
    /**
     *  Constructor
     *
     *  @param value - Pointer to the original Value object
     */
    constexpr Arithmetic(Value *value) : _value(value) {}
    
    /**
     *  Destructor
     */
    virtual ~Arithmetic() = default;
    
    /**
     *  Method to apply a Value object to the referenced Value object and return a
     *  new Value object containing the result of the application
     *
     *  @param  value - The Value object to apply to the referenced Value object
     *
     *  @return Value - A new Value object containing the result of the application
     */
    Value apply(const Value &value)
    {
        // Does the argument represent a floating-point value?
        // If true apply a floating point operation
        if (value.isFloat()) return apply(value.floatValue());
        
        // At this point treat the argument as a 64 bit integer
        // and apply an integral operation
        return resultFromIntegralOperation(value.numericValue());
    }

    /**
     *  Method to apply a 16 bit number to the referenced Value object and return a
     *  new Value object containing the result of the application
     *
     *  @param  value - A 16 bit number to apply to the referenced Value object
     *
     *  @return Value - A new Value object containing the result of the application
     */
    Value apply(int16_t value)
    {
        return resultFromIntegralOperation(value);
    }
    
    /**
     *  Method to apply a 32 bit number to the referenced Value object and return a
     *  new Value object containing the result of the application
     *
     *  @param  value - A 32 bit number to apply to the referenced Value object
     *
     *  @return Value - A new Value object containing the result of the application
     */
    Value apply(int32_t value)
    {
        return resultFromIntegralOperation(value);
    }

    /**
     *  Method to apply a 64 bit number to the referenced Value object and return a
     *  new Value object containing the result of the application
     *
     *  @param  value - A 64 bit number to apply to the referenced Value object
     *
     *  @return Value - A new Value object containing the result of the application
     */
    Value apply(int64_t value)
    {
        return resultFromIntegralOperation(value);
    }
    
    /**
     *  Method to apply a boolean value to the referenced Value object and return a
     *  new Value object containing the result of the application
     *
     *  @param  value - A boolean value to apply to the referenced Value object
     *
     *  @return Value - A new Value object containing the result of the application
     */    
    Value apply(bool value)
    {
        // Does the referenced object represent a floating-point value?
        // If true apply a floating point operation
        if (_value->isFloat()) return apply(static_cast<double>(value? 1 : 0));
        
        // At this point apply an integral operation
        return resultFromIntegralOperation(value? 1 : 0);
    }

    /**
     *  Method to apply character to the referenced Value object and return a
     *  new Value object containing the result of the application
     *
     *  @param  value - A character to apply to the referenced Value object
     *
     *  @return Value - A new Value object containing the result of the application
     */  
    Value apply(char value)
    {
        // Convert argument to a 64 bit integer
        int64_t v = ((value < '0') || (value > '9'))? 0 : (value - '0');

        // Does the referenced object represent a floating-point value?
        // If true apply a floating point operation
        if (_value->isFloat()) return apply(static_cast<double>(v));
        
        // At this point apply an integral operation
        return resultFromIntegralOperation(v);
    }
    
    /**
     *  Method to apply a string to the referenced Value object and return a
     *  new Value object containing the result of the application
     *
     *  Note:
     *     The string must represent a numeric value
     *
     *  @param  value - A string to apply to the referenced Value object
     *
     *  @return Value - A new Value object containing the result of the application
     */  
    Value apply(const std::string &value)
    {
        // Convert the argument to integer
        return resultFromIntegralOperation(atoi(value.c_str()));
    }

    /**
     *  Method to apply a string to the referenced Value object and return a
     *  new Value object containing the result of the application
     *
     *  Note:
     *     The string must represent a numeric value
     *
     *  @param  value - A string to apply to the referenced Value object
     *
     *  @return Value - A new Value object containing the result of the application
     */  
    Value apply(const char *value)
    {
        // Convert the argument to integer
        return resultFromIntegralOperation(atoi(value));
    }

    /**
     *  Method to apply a floating point value to the referenced Value object
     *  and return a new Value object containing the result of the application
     *
     *  @param  value - A floating point value to apply to the referenced Value object
     *
     *  @return Value - A new Value object containing the result of the application
     */ 
    Value apply(const double value)
    {
        // Perform a floating point operation
        return Value{F<double>{}(_value->floatValue(), value)};
    }
    
    /**
     *  Method to assign a Value object to the referenced Value object and return 
     *  the referenced Value object
     *
     *  @param  value - The Value object to assign to the referenced Value object
     *
     *  @return Value - The referenced Value object
     */
    Value &assign(const Value &value)
    {

        // Does the argument represent a floating-point value?
        // If true apply a floating point assignment operation
        if (value.isFloat()) return assign(value.floatValue());
        
        // At this point treat the argument as a 64 bit integer
        // and apply an integral assignment operation
        return manipulatedReferencedValueObject(value.numericValue());
    }

    /**
     *  Method to assign a 16 bit number to the referenced Value object and return 
     *  the referenced Value object
     *
     *  @param  value - A 16 bit number to assign to the referenced Value object
     *
     *  @return Value - The referenced Value object
     */
    Value &assign(int16_t value)
    {
        return manipulatedReferencedValueObject(value);
    }
    
    /**
     *  Method to assign a 32 bit number to the referenced Value object and return 
     *  the referenced Value object
     *
     *  @param  value - A 32 bit number to assign to the referenced Value object
     *
     *  @return Value - The referenced Value object
     */
    Value &assign(int32_t value)
    {
        return manipulatedReferencedValueObject(value);
    }

    /**
     *  Method to assign a 64 bit number to the referenced Value object and return 
     *  the referenced Value object
     *
     *  @param  value - A 64 bit number to assign to the referenced Value object
     *
     *  @return Value - The referenced Value object
     */
    Value &assign(int64_t value)
    {
        return manipulatedReferencedValueObject(value);
    }

    /**
     *  Method to assign a boolean value to the referenced Value object and return 
     *  the referenced Value object
     *
     *  @param  value - A boolean value to assign to the referenced Value object
     *
     *  @return Value - The referenced Value object
     */
    Value &assign(bool value)
    {
        // Does the referenced object represent a floating-point value?
        // If true apply a floating point assignment operation
        if (_value->isFloat()) return assign(static_cast<double>(value? 1 : 0));
        
        // At this point treat the argument as a 64 bit integer
        // and apply an integral assignment operation
        return manipulatedReferencedValueObject(value? 1 : 0);
    }
    
    /**
     *  Method to assign a character to the referenced Value object and return 
     *  the referenced Value object
     *
     *  @param  value - A character to assign to the referenced Value object
     *
     *  @return Value - The referenced Value object
     */
    Value &assign(char value)
    {
        // Convert argument to a 64 bit integer
        int64_t v = ((value < '0') || (value > '9'))? 0 : (value - '0');
        
        // Does the referenced object represent a floating-point value?
        // If true apply a floating point assignment operation
        if (_value->isFloat()) return assign(static_cast<double>(v));
        
        // At this point treat the argument as a 64 bit integer
        // and apply an integral assignment operation
        return manipulatedReferencedValueObject(v);
    }

    /**
     *  Method to assign a string to the referenced Value object and return 
     *  the referenced Value object
     *
     *  Note:
     *     The string must represent a numeric value
     *
     *  @param  value - A string to assign to the referenced Value object
     *
     *  @return Value - The referenced Value object
     */ 
    Value &assign(const std::string &value)
    {
        // Assign an integer
        return manipulatedReferencedValueObject(atoi(value.c_str()));
    }
    
    /**
     *  Method to assign a string to the referenced Value object and return 
     *  the referenced Value object
     *
     *  Note:
     *     The string must represent a numeric value
     *
     *  @param  value - A string to assign to the referenced Value object
     *
     *  @return Value - The referenced Value object
     */ 
    Value &assign(const char *value)
    {
        // Assign an integer
        return manipulatedReferencedValueObject(atoi(value));
    }
    
    /**
     *  Method to assign floating point value to the referenced Value object
     *  and return the referenced Value object
     *
     *  @param  value - A floating point value to assign to the referenced Value object
     *
     *  @return Value - The referenced Value object
     */ 
    Value &assign(const double value)
    {
        // Perform a floating point operation and assignment
        return _value->operator=(F<double>{}(_value->floatValue(), value));
    }
    
private:
    /**
     *  Pointer to the original value object
     */
    Value *_value;

    /**
     *  Implementation detail method to help reduce duplication
     *
     *  Performs an integral operation with the 64 bit argument
     *  and the referenced Value object producing a new Value
     *  object containing the result from the operation
     */
    Value resultFromIntegralOperation(const int64_t value)
    {
        // Does the referenced object represent a floating-point value?
        // If true apply a floating point operation
        if (_value->isFloat()) return apply(static_cast<double>(value));
        
        // At this point perform a 64 bit integral operation
        return Value{F<int64_t>{}(_value->numericValue(), value)};
    }

    /**
     *  Implementation detail method to help reduce duplication
     *
     *  Assigns the 64 bit argument to the referenced Value object
     *  and returns a referened to the Value object
     */
    Value &manipulatedReferencedValueObject(const int64_t value)
    {
        // Does the referenced object represent a floating-point value?
        // If true apply a floating point assignment operation
        if (_value->isFloat()) return assign(static_cast<double>(value));
        
        // At this point perform a 64 bit integral operation and assignment
        return _value->operator=(F<int64_t>{}(_value->numericValue(), value));
    }
};

/**
 *  End of namespace
 */
}
