/**
 *  MethodMember.h
 *
 *  Implementation for a method in a class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class MethodMember : public MemberInfo, public Function
{
public:
    /**
     *  Constructor
     *  @param  name
     *  @param  method
     *  @param  arguments
     */
    MethodMember(const char *name, method_callback_0 method, const std::initializer_list<Argument> &arguments = {}) : Function(name, arguments), _type(0) { _method.m0 = method; }
    MethodMember(const char *name, method_callback_1 method, const std::initializer_list<Argument> &arguments = {}) : Function(name, arguments), _type(1) { _method.m1 = method; }
    MethodMember(const char *name, method_callback_2 method, const std::initializer_list<Argument> &arguments = {}) : Function(name, arguments), _type(2) { _method.m2 = method; }
    MethodMember(const char *name, method_callback_3 method, const std::initializer_list<Argument> &arguments = {}) : Function(name, arguments), _type(3) { _method.m3 = method; }
    MethodMember(const char *name, method_callback_4 method, const std::initializer_list<Argument> &arguments = {}) : Function(name, arguments), _type(4) { _method.m4 = method; }
    MethodMember(const char *name, method_callback_5 method, const std::initializer_list<Argument> &arguments = {}) : Function(name, arguments), _type(5) { _method.m5 = method; }
    MethodMember(const char *name, method_callback_6 method, const std::initializer_list<Argument> &arguments = {}) : Function(name, arguments), _type(6) { _method.m6 = method; }
    MethodMember(const char *name, method_callback_7 method, const std::initializer_list<Argument> &arguments = {}) : Function(name, arguments), _type(7) { _method.m7 = method; }

    /**
     *  Destructor
     */
    virtual ~MethodMember() {}

    /**
     *  Is this a method member
     *  @return bool
     */
    virtual bool isMethod() { return true; }

    /**
     *  Fill a function entry object
     *  @param  entry       Function entry
     *  @param  classname   Name of the class
     */
    virtual void fill(struct _zend_function_entry *entry, const char *classname)
    {
        // call function object
        Function::fill(entry, classname);
    }

private:
    /**
     *  Union of supported callbacks
     *  One of the callbacks will be set 
     */
    union {
        method_callback_0 m0;
        method_callback_1 m1;
        method_callback_2 m2;
        method_callback_3 m3;
        method_callback_4 m4;
        method_callback_5 m5;
        method_callback_6 m6;
        method_callback_7 m7;
    } _method;
    
    /**
     *  The method type that is set
     *  @var integer
     */
    int _type;
};

/**
 *  End of namespace
 */
}

