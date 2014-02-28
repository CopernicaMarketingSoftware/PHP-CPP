/**
 *  MethodMember.h
 *
 *  Implementation for a method in a class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 * 
 *  @todo   remove this class
 *  @todo   but do implement this for properties
 */

/**
 *  Set up namespace
 */
namespace Php {

///**
// *  Class definition
// */
//class MethodMember : public MemberInfo, public Function
//{
//public:
//    /**
//     *  Constructor
//     *  @param  name
//     *  @param  method
//     *  @param  arguments
//     */
//    MethodMember(const char *name, const _Method &method, const std::initializer_list<Argument> &arguments = {}) : Function(name, arguments), _method(method) {}
//
//    /**
//     *  Destructor
//     */
//    virtual ~MethodMember() {}
//
//    /**
//     *  Is this a method member
//     *  @return bool
//     */
//    virtual bool isMethod() { return true; }
//
//    /**
//     *  Fill a function entry object
//     *  @param  entry       Function entry
//     *  @param  classname   Name of the class
//     *  @param  method      Is this a public entry
//     */
//    virtual void fill(struct _zend_function_entry *entry, const char *classname, MemberModifier flags) override
//    {
//        // call function object
//        Function::fill(entry, classname, flags);
//    }
//
//    /**
//     *  Method that gets called every time the function is executed
//     *  @param  params      The parameters that were passed
//     *  @return Variable    Return value
//     */
//    virtual Value invoke(Parameters &params)
//    {
//        return _method.invoke(params);
//    }
//
//private:
//    /**
//     *  The method pointer
//     *  @var _Method
//     */
//    _Method _method;
//};

/**
 *  End of namespace
 */
}

