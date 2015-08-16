/**
 *  @file array.h
 *
 *  This file provides a class that defaults itself to model an array
 *  and upholds this invariant throughout its lifetime
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
     *  Constructor to create an object of this type that defaults itself
     *  to model an array
     */
    Array()
        : Value{ Type::Array }
    {}

    /**
     *  Copy constructor to create an object of this type
     *
     *  If value object's type is not that of an array an exception of type
     *  Php::FatalError is thrown
     *
     *  @param value - Must be a value that models an array
     */
    Array(const Value &value)
        : Value{ value }
    {
        // type must be valid
        if (value.type() != Type::Array) throw FatalError("Assigning a non-array to an array variable");
    }

    /**
     *  Move constructor to create an object of this type
     *
     *  If value object's type is not that of an array an exception of type
     *  Php::FatalError is thrown
     *
     *  @param value - Must be a value that models an array
     */
    Array(Value &&value) _NOEXCEPT
        : Value{ std::move(value) }
    {
        // type must be valid
        if (value.type() != Type::Array) throw FatalError("Moving a non-array to an array variable");
    }

    /**
     *  Constructor to create an object of this type by specifying the following
     *
     *  @param input - A std::vector of elements
     */
    template <typename T>
    Array(const std::vector<T> &input)
        : Value{ input }
    {}

    /**
     *  Constructor that creates an associative array from a std::map of
     *  key-value pairs
     *
     *  @param input - A std::map of key-value pairs
     */
    template <typename T>
    Array(const std::map<std::string,T> &input)
        : Value{ input }
    {}

// Old Visual C++ environments have no support for std::initializer_lists
#   if !defined(_MSC_VER) || (_MSC_VER >= 1800)

    /**
     *  Constructor that creates an array from a std::initializer_list of
     *  Php::Value elements
     *
     *  @param input - A std::initializer_list of Php::Value elements
     */
    Array(const std::initializer_list<Value> &value)
        : Value{ value }
    {}

// End of Visual C++ check
#   endif

    /**
     *  Default destructor
     */
    virtual ~Array() _NOEXCEPT =default;

    /**
     *  This method should not be called within the context of this type as its
     *  only here due to this type's dependency on its parent an only acts as a
     *  guard to uphold an invariant
     *
     *  @param type - Must be that of Type::Array else an exception of Php::FatalError
     *                is thrown
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
     *
     *  If the incoming Php::Value object is not that of Type::Array an exception of
     *  Php::FatalError is thrown
     *
     *  @param  value  - A Php::Value object with a type of Type::Array
     *
     *  @return Array& - This object containing new values corresponding to the
     *                   input argument
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
     *
     *  If the incoming Php::Value object is not that of Type::Array an exception of
     *  FatalError is thrown
     *
     *  @param  value  - A Php::Value object with a type of Type::Array
     *
     *  @return Array& - This object containing new values corresponding to the
     *                   input argument
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
