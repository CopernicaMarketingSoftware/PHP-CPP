/**
 *  Const.h
 *
 *  Class for adding constant properties to a class
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
class Const : public Member
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  value       Default value of the property
     */
    Const(const char *name) : Member(name, MemberModifier::constMember) {}
    Const(const char *name, std::nullptr_t value) : Member(name, MemberModifier::constMember, value) {}
    Const(const char *name, int value) : Member(name, MemberModifier::constMember, value) {}
    Const(const char *name, long value) : Member(name, MemberModifier::constMember, value) {}
    Const(const char *name, bool value) : Member(name, MemberModifier::constMember, value) {}
    Const(const char *name, char value) : Member(name, MemberModifier::constMember, value) {}
    Const(const char *name, const std::string &value) : Member(name, MemberModifier::constMember, value) {}
    Const(const char *name, const char *value, int size=-1) : Member(name, MemberModifier::constMember, value, size) {}
    Const(const char *name, double value) : Member(name, MemberModifier::constMember, value) {}

    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  method      Method to add
     *  @param  arguments   Optional argument information
     */
    Const(const char *name, const _Method &method, const std::initializer_list<Argument> &arguments = {}) : Member(name, MemberModifier::constMember, method, arguments) {}

    /**
     *  Destructor
     */
    virtual ~Const() {}

};

/**
 *  End of namespace
 */
}

