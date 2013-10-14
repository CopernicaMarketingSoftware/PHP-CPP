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
    Public(const char *name) : Member(name, true) {}
    Public(const char *name, std::nullptr_t value) : Member(name, true, value) {}
    Public(const char *name, int value) : Member(name, true, value) {}
    Public(const char *name, long value) : Member(name, true, value) {}
//    Public(const char *name, bool value) : Member(name, true, value) {}
    Public(const char *name, char value) : Member(name, true, value) {}
    Public(const char *name, const std::string &value) : Member(name, true, value) {}
    Public(const char *name, const char *value, int size=-1) : Member(name, true, value, size) {}
    Public(const char *name, double value) : Member(name, true, value) {}

    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  method      Method to add
     */
    Public(const char *name, method_callback_0 method, const std::initializer_list<Argument> &arguments = {}) : Member(name, true, method, arguments) {}
    Public(const char *name, method_callback_1 method, const std::initializer_list<Argument> &arguments = {}) : Member(name, true, method, arguments) {}
    Public(const char *name, method_callback_2 method, const std::initializer_list<Argument> &arguments = {}) : Member(name, true, method, arguments) {}
    Public(const char *name, method_callback_3 method, const std::initializer_list<Argument> &arguments = {}) : Member(name, true, method, arguments) {}
    Public(const char *name, method_callback_4 method, const std::initializer_list<Argument> &arguments = {}) : Member(name, true, method, arguments) {}
    Public(const char *name, method_callback_5 method, const std::initializer_list<Argument> &arguments = {}) : Member(name, true, method, arguments) {}
    Public(const char *name, method_callback_6 method, const std::initializer_list<Argument> &arguments = {}) : Member(name, true, method, arguments) {}
    Public(const char *name, method_callback_7 method, const std::initializer_list<Argument> &arguments = {}) : Member(name, true, method, arguments) {}

    /**
     *  Destructor
     */
    virtual ~Public() {}
    
    
};

/**
 *  End of namespace
 */
}

