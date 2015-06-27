/**
 *  @file byref.h
 *
 *  This class models the semantic meaning of accepting an argument by reference
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT ByRef : public Argument
{
public:
    /**
     *  Constructor to create an argument by reference specifying the following
     *
     *  @param name     Name of the argument
     *  @param type     The argument's type
     *  @param required Is this argument required?
     */
    ByRef(const char *name, Type type = Type::Null, bool required = true)
        : Argument{name, type, required, true}
    {}

    /**
     *  Constructor
     *  @param  name        Name of the argument
     *  @param  classname   Name of the class
     *  @param  nullable    Can it be null?
     *  @param  required    Is this argument required?
     */
    ByRef(const char *name, const char *classname, bool nullable = false, bool required = true)
        : Argument{name, classname, nullable, required, true}
    {}

    /**
     *  Copy constructor
     *  @param  argument
     */
    ByRef(const ByRef &argument) : Argument(argument) {}

    /**
     *  Move constructor
     *  @param  argument
     */
    ByRef(ByRef &&argument) _NOEXCEPT : Argument(argument) {}

    /**
     *  Destructor
     */
    virtual ~ByRef() {}
};

/**
 *  End of namespace
 */
}
