/**
 *  @file array.h
 *
 *  Array is a value that defaults itself to model an array and upholds this
 *  invariant throughout its lifetime
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
class PHPCPP_EXPORT Array : public Value
{
public:
    /**
     *  Constructor that defaults itself to model an array
     */
    Array() : Value(Type::Array) {}

    /**
     *  Copy constructor from a value object
     *
     *  If value object's type is not that of an array an exception of type
     *  FatalError is thrown
     *
     *  @param value Must be a value that models an array
     */
    Array(const Value &value) : Value(value)
    {
        // type must be valid
        if (value.type() != Type::Array) throw FatalError("Assigning a non-array to an array variable");
    }

    /**
     *  Move constructor from a value object
     *
     *  If value object's type is not that of an array an exception of type
     *  FatalError is thrown
     *
     *  @param value Must be a value that models an array
     */
    Array(Value &&value)  _NOEXCEPT : Value(std::move(value))
    {
        // type must be valid
        if (value.type() != Type::Array) throw FatalError("Moving a non-array to an array variable");
    }

    /**
     *  Constructor that creates an array from a std::vector of elements
     *
     *  @param input A std::vector of elements
     */
    template <typename T>
    Array(const std::vector<T> &input) : Value(input) {}

    /**
     *  Constructor that creates an associative array from a std::map of
     *  key-value pairs
     *
     *  @param input A std::map of key-value pairs
     */
    template <typename T>
    Array(const std::map<std::string,T> &input) : Value(input) {}

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
