/**
 *  @file byval.h
 *
 *  This file provides a class that models the semantics of accepting an argument by value
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @author Rico Antonio Felix <ricoantoniofelix@yahoo.com>
 *
 *  @copyright 2013-2015 Copernica BV
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT ByVal : public Argument
{
public:
    /**
     *  Constructor to create an object of this type by specifying the following
     *
     *  @param name     - What is the argument's name?
     *  @param type     - What type of argument is this?
     *  @param required - Is this argument required?
     */
    ByVal(const char * name, Type type = Type::Null, bool required = true) _NOEXCEPT
        : Argument{ name, type, required, false }
    {}

    /**
     *  Constructor to create an object of this type by specifying the following
     *
     *  @param name      - What is the argument's name?
     *  @param classname - What is the name of the class this argument represents?
     *  @param nullable  - Can it be null?
     *  @param required  - Is this argument required?
     */
    ByVal(const char * name, const char * classname, bool nullable = false, bool required = true) _NOEXCEPT
        : Argument{ name, classname, nullable, required, false }
    {}

    /**
     *  Copy constructor to create an object of this type by specifying the following
     *
     *  @param argument - Where is the object of this type to copy from?
     */
    ByVal(const ByVal & argument) _NOEXCEPT
        : Argument{ argument }
    {}

    /**
     *  Move constructor to create an object of this type by specifying the following
     *
     *  @param argument - Where is the object of this type to steal its possessions?
     */
    ByVal(ByVal && argument) _NOEXCEPT
        : Argument{ argument }
    {}

    /**
     *  Default destructor
     */
    virtual ~ByVal() _NOEXCEPT =default;
};

/**
 *  End of namespace
 */
}
