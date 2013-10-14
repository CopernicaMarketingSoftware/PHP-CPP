/**
 *  Member.cpp
 *
 *  Implementation for class members
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 */
Member::Member(const char *name, bool pub) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;
    
    // create a null member
    _info = new NullMember();
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 */
Member::Member(const char *name, bool pub, std::nullptr_t value) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create a null member
    _info = new NullMember();
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 */
Member::Member(const char *name, bool pub, int value) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create a long member
    _info = new LongMember(value);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 */
Member::Member(const char *name, bool pub, long value) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create a long member
    _info = new LongMember(value);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 */
Member::Member(const char *name, bool pub, bool value) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create a bool member
    _info = new BoolMember(value);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 */
Member::Member(const char *name, bool pub, char value) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create a new string member
    _info = new StringMember(&value, 1);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 */
Member::Member(const char *name, bool pub, const std::string &value) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create a new string member
    _info = new StringMember(value);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 *  @param  size        String length
 */
Member::Member(const char *name, bool pub, const char *value, int size) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create a new string member
    if (size < 0) size = strlen(value);
    _info = new StringMember(value, size);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 */
Member::Member(const char *name, bool pub, double value) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create a new double member
    _info = new DoubleMember(value);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  method      The method to call
 *  @param  arguments   Argument meta data
 */
Member::Member(const char *name, bool pub, method_callback_0 method, const std::initializer_list<Argument> &arguments) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create method member
    _info = new MethodMember(name, method, arguments);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  method      The method to call
 *  @param  arguments   Argument meta data
 */
Member::Member(const char *name, bool pub, method_callback_1 method, const std::initializer_list<Argument> &arguments) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;
    
    // create method member
    _info = new MethodMember(name, method, arguments);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  method      The method to call
 *  @param  arguments   Argument meta data
 */
Member::Member(const char *name, bool pub, method_callback_2 method, const std::initializer_list<Argument> &arguments) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create method member
    _info = new MethodMember(name, method, arguments);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  method      The method to call
 *  @param  arguments   Argument meta data
 */
Member::Member(const char *name, bool pub, method_callback_3 method, const std::initializer_list<Argument> &arguments) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create method member
    _info = new MethodMember(name, method, arguments);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  method      The method to call
 *  @param  arguments   Argument meta data
 */
Member::Member(const char *name, bool pub, method_callback_4 method, const std::initializer_list<Argument> &arguments) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create method member
    _info = new MethodMember(name, method, arguments);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  method      The method to call
 *  @param  arguments   Argument meta data
 */
Member::Member(const char *name, bool pub, method_callback_5 method, const std::initializer_list<Argument> &arguments) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create method member
    _info = new MethodMember(name, method, arguments);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  method      The method to call
 *  @param  arguments   Argument meta data
 */
Member::Member(const char *name, bool pub, method_callback_6 method, const std::initializer_list<Argument> &arguments) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create method member
    _info = new MethodMember(name, method, arguments);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  method      The method to call
 *  @param  arguments   Argument meta data
 */
Member::Member(const char *name, bool pub, method_callback_7 method, const std::initializer_list<Argument> &arguments) : _name(name), _public(pub)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // create method member
    _info = new MethodMember(name, method, arguments);
}

/**
 *  Copy constructor
 *  @param  member      The member to copy
 */
Member::Member(const Member &member)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // copy info object, and name and public members
    _info = member._info;
    _name = member._name;
    _public = member._public;
    
    // update refcount in info object
    _info->refcount(+1);
}

/**
 *  Move constructor
 *  @param  member      The member to move
 */
Member::Member(Member &&member)
{
    std::cout << "Allocate member " << __LINE__ << std::endl;

    // move info object, and name and public properties
    _info = member._info;
    _name = std::move(member._name);
    _public = member._public;
    
    // reset info in other object
    member._info = NULL;
}

/**
 *  Destructor
 */
Member::~Member()
{
    // leap out if there is no other info object, or when it is also used by other objects
    if (!_info || _info->refcount(-1) > 0) return;
    
    // deallocate info object
    delete _info;
}

/**
 *  Is this a property member
 *  @return bool
 */
bool Member::isProperty()
{
    return _info && _info->isProperty();
}

/**
 *  Is this a method member
 *  @return bool
 */
bool Member::isMethod()
{
    std::cout << "call isMethod" << std::endl;
    
    return _info && _info->isMethod();
}

/**
 *  Internal method to declare the property
 *  @var zend_class_entry
 */
void Member::declare(struct _zend_class_entry *entry)
{
    // let the info object handle stuff
    _info->declare(entry, _name.c_str(), _name.size(), _public ? ZEND_ACC_PUBLIC : ZEND_ACC_PROTECTED TSRMLS_CC);
}

/**
 *  Internal method to fill a function entry
 *  @param  zend_function_entry
 *  @param  classname
 *  @internal
 */
void Member::fill(struct _zend_function_entry *entry, const char *classname)
{
    // let the info object do this
    _info->fill(entry, classname);
}
    
/**
 *  End of namespace
 */
}

