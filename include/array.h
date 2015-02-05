/**
 *  Array.h
 *
 *  The Array is a wrapper around the value class that ensures that a 
 *  certain property always is an array
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013, 2014 Copernica BV
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
    Array() : Value(Type::Array) {}
    
    /**
     *  Copy constructor from a value object
     *  @param  value
     */
    Array(const Value &value) : Value(value) 
    {
        // type must be valid
        if (value.type() != Type::Array) throw FatalError("Assigning a non-array to an array variable");
    }

    /**
     *  Move constructor from a value object
     *  @param  value
     */
    Array(Value &&value)  _NOEXCEPT : Value(std::move(value))
    {
        // type must be valid
        if (value.type() != Type::Array) throw FatalError("Moving a non-array to an array variable");
    }

    /**
     *  Constructors from a vector (this will create an array)
     *  @param  value
     */
    template <typename T>
    Array(const std::vector<T> &input) : Value(input) {}
    
    /**
     *  Constructor from a map (this will create an associative array)
     *  @param  value
     */
    template <typename T>
    Array(const std::map<std::string,T> &value) : Value(value) {}

// old visual c++ environments have no support for initializer lists
#   if !defined(_MSC_VER) || _MSC_VER >= 1800

    /**
     *  Constructor from an initializer list
     *  @param  value
     */
    Array(const std::initializer_list<Value> &value) : Value(value) {}

// end of visual c++ check
#   endif  
    
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
        // throw exception if things are going wrong
        if (type != Type::Array) throw FatalError("Changing type of a fixed array variable");
        
        // call base
        return Value::setType(Type::Array);
    }
    
    /**
     *  Assignment operator
     *  @param  value
     *  @return Array
     */
    Array &operator=(const Value &value)
    {
        // skip self assignment
        if (this == &value) return *this;
        
        // type must be valid
        if (value.type() != Type::Array) throw FatalError("Assigning a non-array to a fixed array variable");
        
        // call base
        Value::operator=(value);

        // done
        return *this;
    }
    
    /**
     *  Move assignment operator
     *  @param  value
     *  @return Array
     */
    Array &operator=(Value &&value) _NOEXCEPT
    {
        // skip self assignment
        if (this == &value) return *this;
        
        // type must be valid
        if (value.type() != Type::Array) throw FatalError("Moving a non-array to a fixed array variable");
        
        // call base
        Value::operator=(std::move(value));
        
        // done
        return *this;
    }
};

/**
 *  End of namespace
 */
}

