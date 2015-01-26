/**
 *  ByVal.h
 *
 *  Overridden Argument class to specify by-value function arguments
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
class ByVal : public Argument
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the argument
     *  @param  type        Argument type
     *  @param  required    Is this argument required?
     */
    ByVal(const char *name, Type type = Type::Null, bool required = true) : Argument(name, type, required, false) {}

    /**
     *  Constructor
     *  @param  name        Name of the argument
     *  @param  classname   Name of the class
     *  @param  nullable    Can it be null?
     *  @param  required    Is this argument required?
     */
    ByVal(const char *name, const char *classname, bool nullable = false, bool required = true) : Argument(name, classname, nullable, required, false) {}

    /**
     *  Copy constructor
     *  @param  argument
     */
    ByVal(const ByVal &argument) : Argument(argument) {}

    /**
     *  Move constructor
     *  @param  argument
     */
    ByVal(ByVal &&argument) _NOEXCEPT : Argument(argument) {}

    /**
     *  Destructor
     */
    virtual ~ByVal() {}
};

/**
 *  End of namespace
 */
}

