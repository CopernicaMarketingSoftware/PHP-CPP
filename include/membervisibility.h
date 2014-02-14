/**
 *  membervisibility.h
 *
 *  MemberVisibility  - Template for a visibility of a property or method
 *  Instead of defining three classes Public, Protected and Private defined template class. And these classes are obtained by applying to the class template typedef MemberVisibility. 
 *  In the future, so it is possible to create such a class scope `Static`
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  changed by Valeriy Dmitriev <ufabiz@gmail.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  Class definition
 */
template <Zend::AccMemb AccFlag>
class MemberVisibility : public Member
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  value       Default value of the property
     */
    MemberVisibility(const char *name) : Member(name, FlagMemb(AccFlag)) {}
    MemberVisibility(const char *name, std::nullptr_t value) : Member(name, FlagMemb(AccFlag), value) {}
    MemberVisibility(const char *name, int value) : Member(name, FlagMemb(AccFlag), value) {}
    MemberVisibility(const char *name, long value) : Member(name, FlagMemb(AccFlag), value) {}
    MemberVisibility(const char *name, bool value) : Member(name, FlagMemb(AccFlag), value) {}
    MemberVisibility(const char *name, char value) : Member(name, FlagMemb(AccFlag), value) {}
    MemberVisibility(const char *name, const std::string &value) : Member(name, FlagMemb(AccFlag), value) {}
    MemberVisibility(const char *name, const char *value, int size=-1) : Member(name, FlagMemb(AccFlag), value, size) {}
    MemberVisibility(const char *name, double value) : Member(name, FlagMemb(AccFlag), value) {}

    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  method      Method to add
     *  @param  arguments   Optional argument information
     */
    MemberVisibility(const char *name, const _Method &method, const std::initializer_list<Argument> &arguments = {}) : Member(name, FlagMemb(AccFlag), method, arguments) {}
    
    /**
     *  Destructor
     */
    virtual ~MemberVisibility() {}
    
};

typedef MemberVisibility<Zend::AccMemb::PUBLIC> Public;
typedef MemberVisibility<Zend::AccMemb::PROTECTED> Protected;
typedef MemberVisibility<Zend::AccMemb::PRIVATE> Private;
typedef MemberVisibility<Zend::AccMemb::CONSTANT> Const;

/**
 *  In the current architecture, implementation of static methods is not possible.
 *  
 *  Static properties are supported.
 *  @todo: Requires some refactoring that it became possible.
 */
//typedef MemberVisibility<Zend::AccMemb::STATIC> Static;


/**
 *  End of namespace
 */
}

