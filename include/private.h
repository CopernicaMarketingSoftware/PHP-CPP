/**
 *  Private.h
 *
 *  Class for adding private properties to a class
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
class Private : public Member
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  value       Default value of the property
     */
    Private(const char *name) : Member(name, MemberModifier::privateMember) {}
    Private(const char *name, std::nullptr_t value) : Member(name, MemberModifier::privateMember, value) {}
    Private(const char *name, int value) : Member(name, MemberModifier::privateMember, value) {}
    Private(const char *name, long value) : Member(name, MemberModifier::privateMember, value) {}
    Private(const char *name, bool value) : Member(name, MemberModifier::privateMember, value) {}
    Private(const char *name, char value) : Member(name, MemberModifier::privateMember, value) {}
    Private(const char *name, const std::string &value) : Member(name, MemberModifier::privateMember, value) {}
    Private(const char *name, const char *value, int size=-1) : Member(name, MemberModifier::privateMember, value, size) {}
    Private(const char *name, double value) : Member(name, MemberModifier::privateMember, value) {}

    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  method      Method to add
     *  @param  arguments   Optional argument information
     */
    Private(const char *name, const _Method &method, const std::initializer_list<Argument> &arguments = {}) : Member(name, MemberModifier::privateMember, method, arguments) {}

    /**
     *  Destructor
     */
    virtual ~Private() {}

};

/**
 *  End of namespace
 */
}

