/**
 *  Arithmethic.h
 *
 *  Helper class that takes care of arithmetic operations on PHP variables
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
template < template<typename T> class F>
class Arithmetic
{
public:
    /**
     *  Constructor
     *  @param  value       The original object
     */
    Arithmetic(Value *value) : _value(value) {}
    
    /**
     *  Destructor
     */
    virtual ~Arithmetic() {}
    
    /**
     *  Apply a number, and return a new value object after running the arithmetic function
     *  @param  value
     *  @return Value
     */
    Value apply(const Value &value)
    {
        // is this a type a floating point type?
        if (value.isFloat()) return apply(value.floatValue());
        
        // we are going to treat it as a numeric (non floating) type
        return apply(value.numericValue());
    }
    
    /**
     *  Apply a number, and return a new value object after running the arithmetic function
     *  @param  value
     *  @return Value
     */
    Value apply(int16_t value)
    {
        // check if the current object is a floating point number
        if (_value->isFloat()) return Value(F<double>()(_value->floatValue(), value));
        
        // apply to natural numbers
        return Value(F<int64_t>()(_value->numericValue(), value));
    }
    
    /**
     *  Apply a number, and return a new value object after running the arithmetic function
     *  @param  value
     *  @return Value
     */
    Value apply(int32_t value)
    {
        // check if the current object is a floating point number
        if (_value->isFloat()) return Value(F<double>()(_value->floatValue(), value));
        
        // apply to natural numbers
        return Value(F<int64_t>()(_value->numericValue(), value));
    }

    /**
     *  Apply a number, and return a new value object after running the arithmetic function
     *  @param  value
     *  @return Value
     */
    Value apply(int64_t value)
    {
        // check if the current object is a floating point number
        if (_value->isFloat()) return Value(F<double>()(_value->floatValue(), value));
        
        // apply to natural numbers
        return Value(F<int64_t>()(_value->numericValue(), value));
    }
        
    /**
     *  Apply a number, and return a new value object after running the arithmetic function
     *  @param  value
     *  @return Value
     */
    Value apply(long value)
    {
        // check if the current object is a floating point number
        if (_value->isFloat()) return Value(F<double>()(_value->floatValue(), value));
        
        // apply to natural numbers
        return Value(F<long>()(_value->numericValue(), value));
    }

    /**
     *  Apply a boolean (treat is as 0 or 1), and return a new value object after running the arithmetic function
     *  @param  value
     *  @return Value
     */
    Value apply(bool value)
    {
        // check if the current object is a floating point number
        if (_value->isFloat()) return Value(F<double>()(_value->floatValue(), value?1:0));
        
        // apply to natural numbers
        return Value(F<int64_t>()(_value->numericValue(), value?1:0));
    }
    
    /**
     *  Apply a character (value between '0' and '9'), and return a new value object after running the arithmetic function
     *  @param  value
     *  @return Value
     */
    Value apply(char value)
    {
        // convert to an integer
        int v = value < '0' || value > '9' ? 0 : value - '0';

        // check if the current object is a floating point number
        if (_value->isFloat()) return Value(F<double>()(_value->floatValue(), v));
        
        // apply to natural numbers
        return Value(F<int64_t>()(_value->numericValue(), v));
    }
    
    /**
     *  Apply a string (representing a number), and return a new value object after running the arithmetic function
     *  @param  value
     *  @return Value
     */
    Value apply(const std::string &value)
    {
        // convert string to integer
        return apply(atoi(value.c_str()));
    }

    /**
     *  Apply a string (representing a number), and return a new value object after running the arithmetic function
     *  @param  value
     *  @return Value
     */
    Value apply(const char *value)
    {
        // convert string to integer
        return apply(atoi(value));
    }

    /**
     *  Apply a string (representing a number), and return a new value object after running the arithmetic function
     *  @param  value
     *  @return Value
     */
    Value apply(double value)
    {
        return Value(F<double>()(_value->floatValue(), value));
    }
    
    /**
     *  Assign a different value object, applying the arithmetic operation
     *  @param  value
     *  @return Value
     */
    Value &assign(const Value &value)
    {
        // is this a type a floating point type?
        if (value.isFloat()) return assign(value.floatValue());
        
        // we are going to treat it as a numeric (non floating) type
        return assign(value.numericValue());
    }

    /**
     *  Assign a 16bit number, applying the arithmetic operation
     *  @param  value
     *  @return Value
     */
    Value &assign(int16_t value)
    {
        // is the current object a floating point type?
        if (_value->isFloat()) return _value->operator=(F<double>()(_value->floatValue(), value));
        
        // do a numeric operation
        return _value->operator=(F<int64_t>()(_value->numericValue(), value));
    }
    
    /**
     *  Assign 32bit integer, applying the arithmetic operation
     *  @param  value
     *  @return Value
     */
    Value &assign(int32_t value)
    {
        // is the current object a floating point type?
        if (_value->isFloat()) return _value->operator=(F<double>()(_value->floatValue(), value));
        
        // do a numeric operation
        return _value->operator=(F<int64_t>()(_value->numericValue(), value));
    }

    /**
     *  Assign 64bit integer, applying the arithmetic operation
     *  @param  value
     *  @return Value
     */
    Value &assign(int64_t value)
    {
        // is the current object a floating point type?
        if (_value->isFloat()) return _value->operator=(F<double>()(_value->floatValue(), value));
        
        // do a numeric operation
        return _value->operator=(F<int64_t>()(_value->numericValue(), value));
    }

    /**
     *  Assign long int, applying the arithmetic operation
     *  @param  value
     *  @return Value
     */
    Value &assign(long value)
    {
        // is the current object a floating point type?
        if (_value->isFloat()) return _value->operator=(F<double>()(_value->floatValue(), value));
        
        // do a numeric operation
        return _value->operator=(F<long>()(_value->numericValue(), value));
    }

    /**
     *  Assign 64bit integer - which is treated as 1 or 0 - applying the arithmetic operation
     *  @param  value
     *  @return Value
     */
    Value &assign(bool value)
    {
        // is the current object a floating point type?
        if (_value->isFloat()) return _value->operator=(F<double>()(_value->floatValue(), value?1:0));
        
        // do a numeric operation
        return _value->operator=(F<int64_t>()(_value->numericValue(), value?1:0));
    }
    
    /**
     *  Assign a single character - which is treated as an int, and applying the arithmetic function
     *  @param  value
     *  @return Value
     */
    Value &assign(char value)
    {
        // convert to an integer
        int v = value < '0' || value > '9' ? 0 : value - '0';
        
        // is the current object a floating point type?
        if (_value->isFloat()) return _value->operator=(F<double>()(_value->floatValue(), v));
        
        // do a numeric operation
        return _value->operator=(F<int64_t>()(_value->numericValue(), v));
    }

    /**
     *  Assign a a string - treating it as an integer, and applying the arithmetic function
     *  @param  value
     *  @return Value
     */
    Value &assign(const std::string &value)
    {
        // assign integer
        return assign(atoi(value.c_str()));
    }
    
    /**
     *  Assign a string - treating it as an integer, and applying the arithmetic function
     *  @param  value
     *  @return Value
     */
    Value &assign(const char *value)
    {
        // assign integer
        return assign(atoi(value));
    }
    
    /**
     *  Assign a double, applying the arithmetic operation
     *  @param  value
     *  @return Value
     */
    Value &assign(double value)
    {
        // do float operation
        return _value->operator=(F<double>()(_value->floatValue(), value));
    }
    
private:
    /**
     *  Pointer to the original value object
     *  @var    Value
     */
    Value *_value;
    

};

/**
 *  End of namespace
 */
}

