/**
 *  Protected.h
 *
 *  Class for adding protected properties to a class
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
class Protected : public Member
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  value       Default value of the property
     */
    Protected(const char *name) : Member(name, MemberModifier::protectedMember) {}
    Protected(const char *name, std::nullptr_t value) : Member(name, MemberModifier::protectedMember, value) {}
    Protected(const char *name, int value) : Member(name, MemberModifier::protectedMember, value) {}
    Protected(const char *name, long value) : Member(name, MemberModifier::protectedMember, value) {}
    Protected(const char *name, bool value) : Member(name, MemberModifier::protectedMember, value) {}
    Protected(const char *name, char value) : Member(name, MemberModifier::protectedMember, value) {}
    Protected(const char *name, const std::string &value) : Member(name, MemberModifier::protectedMember, value) {}
    Protected(const char *name, const char *value, int size=-1) : Member(name, MemberModifier::protectedMember, value, size) {}
    Protected(const char *name, double value) : Member(name, MemberModifier::protectedMember, value) {}

    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  method      Method to add
     *  @param  arguments   Optional argument information
     */
    Protected(const char *name, const _Method &method, const std::initializer_list<Argument> &arguments = {}) : Member(name, MemberModifier::protectedMember, method, arguments) {}

    /**
     *  Destructor
     */
    virtual ~Protected() {}

};

/**
 *  End of namespace
 */
}

