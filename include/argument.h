/**
 *  @file argument.h
 *
 *  This file provides a class that holds information about an argument passed to a function.
 *
 *  You'll need this class when you're defining your own functions.
 *
 *  The constructor of the class is protected so clients are directed to use the Php::ByVal or
 *  Php::ByRef classes instead.
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
class PHPCPP_EXPORT Argument
{
public:
    /**
     *  Default Destructor
     */
    virtual ~Argument() =default;

    /**
     *  Method stating whether this is a required argument or not
     *
     *  @return bool - true if the argument is required, false otherwise
     *  @internal
     */
    bool required() const _NOEXCEPT
    {
        return m_required;
    }

    /**
     *  Method to retrieve the name of the argument
     *
     *  @return const char * - C-String containing the name of the argument
     */
    const char *name() const _NOEXCEPT
    {
        return m_name;
    }

    /**
     *  Method to retrieve the type of the argument
     *
     *  @return Php::Type - The type this argument is mapped to
     */
    Type type() const _NOEXCEPT
    {
        return m_type;
    }

    /**
     *  Method to retrieve the class name if the type is a class
     *
     *  @return const char * - C-String containing the name of the class if the
     *                         argument's type is that of a class
     */
    const char *classname() const _NOEXCEPT
    {
        return m_classname;
    }

    /**
     *  Method stating whether it is allowed to pass parameter with a null value
     *
     *  @return bool - true if can be nullable, false otherwise
     */
    bool allowNull() const _NOEXCEPT
    {
        return m_nullable;
    }

    /**
     *  Method stating whether this a parameter-by-reference
     *
     *  @return bool - true if its a reference, false otherwise
     */
    bool byReference() const _NOEXCEPT
    {
        return m_byReference;
    }

protected:
    /**
     *  Constructor to create an object of this type by specifying the following
     *
     *  @param name     - Argument name
     *  @param type     - Argument type
     *  @param required - Is this argument required?
     *  @param byref    - Is this a reference argument?
     */
    Argument(const char *name, Type type, bool required = true, bool byref = false)
        : m_name        { name     }
        , m_type        { type     }
        , m_classname   { nullptr  }
        , m_nullable    { false    }
        , m_required    { required }
        , m_byReference { byref    }
    {}

    /**
     *  Constructor to create an object of this type by specifying the following
     *
     *  @param name      - Argument name
     *  @param classname - Class name
     *  @param nullable  - Can it be null?
     *  @param required  - Is this argument required?
     *  @param byref     - Is this a reference argument?
     */
    Argument(const char *name, const char *classname, bool nullable = true,
             bool required = true, bool byref = false)
        : m_name        { name         }
        , m_type        { Type::Object }
        , m_classname   { classname    }
        , m_nullable    { nullable     }
        , m_required    { required     }
        , m_byReference { byref        }
    {}

private:
    /**
     *  Argument name
     */
    const char * m_name;

    /**
     *  Type of argument
     */
    Type m_type;

    /**
     *  The name of the class if this is a parameter that is supposed to be
     *  an instance of a class
     */
    const char * m_classname;

    /**
     *  May the parameter be null?
     */
    bool m_nullable;

    /**
     *  Is this a required argument?
     */
    bool m_required;

    /**
     *  Is this a 'by-reference' parameter?
     */
    bool m_byReference;
};

/**
 *  Old Visual C++ environments do not support initializer lists,
 *  therefore arguments should be passed as vectors
 */
#if defined(_MSC_VER) && (_MSC_VER < 1800)

/**
 *  A vector of arguments can be supplied to methods
 *
 *  @type Arguments - A vector of arguments
 */
using Arguments = std::vector<Argument>;

/**
 *  Other compilers, and visual C++ 2013 do support initializer lists
 */
#else

/**
 *  A list of arguments can be supplied to methods
 *
 *  @type Arguments - A list of arguments
 */
using Arguments = std::initializer_list<Argument>;

/**
 *  End of visual C++ check
 */
#endif

/**
 *  End of namespace
 */
}
