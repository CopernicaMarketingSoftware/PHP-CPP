/**
 *  Public.h
 *
 *  Class for adding public properties to a class
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
class Public : public Member
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  value       Default value of the property
     */
    Public(const char *name) : Member(name, Flag(Zend::AccMemb::PUBLIC)) {}
    Public(const char *name, std::nullptr_t value) : Member(name, Flag(Zend::AccMemb::PUBLIC), value) {}
    Public(const char *name, int value) : Member(name, Flag(Zend::AccMemb::PUBLIC), value) {}
    Public(const char *name, long value) : Member(name, Flag(Zend::AccMemb::PUBLIC), value) {}
    Public(const char *name, bool value) : Member(name, Flag(Zend::AccMemb::PUBLIC), value) {}
    Public(const char *name, char value) : Member(name, Flag(Zend::AccMemb::PUBLIC), value) {}
    Public(const char *name, const std::string &value) : Member(name, Flag(Zend::AccMemb::PUBLIC), value) {}
    Public(const char *name, const char *value, int size=-1) : Member(name, Flag(Zend::AccMemb::PUBLIC), value, size) {}
    Public(const char *name, double value) : Member(name, Flag(Zend::AccMemb::PUBLIC), value) {}

    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  method      Method to add
     *  @param  arguments   Optional argument information
     */
    Public(const char *name, const _Method &method, const std::initializer_list<Argument> &arguments = {}) : Member(name, Flag(Zend::AccMemb::PUBLIC), method, arguments) {}

    /**
     *  Destructor
     */
    virtual ~Public() {}
    
    
};

/**
 *  End of namespace
 */
}

