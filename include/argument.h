/**
 *  @file argument.h
 *
 *  This file provides a class that holds information about an argument passed to a function.
 *
 *  You'll need this class when you're defining your own functions.
 *
 *  The constructors of the class are protected so clients are directed to use the related
 *  Php::ByVal or Php::ByRef classes instead.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
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
class PHPCPP_EXPORT Argument
{
public:
    /**
     *  Default destructor
     */
    virtual ~Argument() _NOEXCEPT =default;

    /**
     *  Method stating whether this is a required argument or not
     *
     *  @return bool - true if the argument is required, false otherwise
     *  @internal
     */
    bool required() const _NOEXCEPT
    {
        return _required;
    }

    /**
     *  Method to retrieve the name of the argument
     *
     *  @return const char * - C-String containing the name of the argument
     */
    const char * name() const _NOEXCEPT
    {
        return _name;
    }

    /**
     *  Method to retrieve the type of the argument
     *
     *  @return Php::Type - The type this argument is mapped to
     */
    Type type() const _NOEXCEPT
    {
        return _type;
    }

    /**
     *  Method to retrieve the class name if the type is a class
     *
     *  @return const char * - C-String containing the name of the class if the
     *                         argument's type is that of a class
     */
    const char * classname() const _NOEXCEPT
    {
        return _classname;
    }

    /**
     *  Method stating whether it is allowed to pass the argument with a null value
     *
     *  @return bool - true if can be null, false otherwise
     */
    bool allowNull() const _NOEXCEPT
    {
        return _nullable;
    }

    /**
     *  Method stating whether this a by-reference argument
     *
     *  @return bool - true if its a reference, false otherwise
     */
    bool byReference() const _NOEXCEPT
    {
        return _byReference;
    }

protected:
    /**
     *  Constructor to create an object of this type by specifying the following
     *
     *  @param name     - What is the argument's name?
     *  @param type     - What type of argument is this?
     *  @param required - Is this argument required?
     *  @param byref    - Is this a reference argument?
     */
    Argument(const char * name, Type type, bool required = true, bool byref = false) _NOEXCEPT
        : _name        { name     }
        , _type        { type     }
        , _classname   { nullptr  }
        , _nullable    { false    }
        , _required    { required }
        , _byReference { byref    }
    {}

    /**
     *  Constructor to create an object of this type by specifying the following
     *
     *  @param name      - What is the argument's name?
     *  @param classname - What is the name of the class this argument represents?
     *  @param nullable  - Can it be null?
     *  @param required  - Is this argument required?
     *  @param byref     - Is this a reference argument?
     */
    Argument(const char * name, const char * classname, bool nullable = true, bool required = true, bool byref = false) _NOEXCEPT
        : _name        { name         }
        , _type        { Type::Object }
        , _classname   { classname    }
        , _nullable    { nullable     }
        , _required    { required     }
        , _byReference { byref        }
    {}

    /**
     *  Copy constructor to create an object of this type by specifying the following
     *
     *  @param argument - Where is the object of this type to copy from?
     */
     Argument(const Argument & argument) _NOEXCEPT =default;

     /**
     *  Move constructor to create an object of this type by specifying the following
     *
     *  @param argument - Where is the object of this type to steal its possessions?
     */
     Argument(Argument && argument) _NOEXCEPT =default;

private:
    /**
     *  The argument's name
     */
    const char * _name;

    /**
     *  The type of argument
     */
    Type _type;

    /**
     *  The name of the class if this is an argument that is supposed to be
     *  an instance of a class
     */
    const char * _classname;

    /**
     *  The answer to whether the argument can be null
     */
    bool _nullable;

    /**
     *  The answer to whether this argument is required
     */
    bool _required;

    /**
     *  The answer to whether this is a 'by-reference' argument
     */
    bool _byReference;
};

/**
 *  Old Visual C++ environments do not support std::initializer_list,
 *  therefore arguments should be passed as a std::vector
 */
#if defined(_MSC_VER) && (_MSC_VER < 1800)

/**
 *  A vector of arguments can be supplied to methods
 *
 *  @type Arguments - A vector of arguments
 */
using Arguments = std::vector<Argument>;

/**
 *  Other compilers, and Visual C++ 2013 do support std::initializer_list
 */
#else

/**
 *  A list of arguments can be supplied to methods
 *
 *  @type Arguments - A list of arguments
 */
using Arguments = std::initializer_list<Argument>;

/**
 *  End of Visual C++ check
 */
#endif

/**
 *  End of namespace
 */
}
