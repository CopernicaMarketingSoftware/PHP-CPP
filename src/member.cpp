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
Member::Member(const char *name, bool pub) : _name(name), _public(pub), _constant(false)
{
    // create a null member
    _info = new NullMember();
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 */
Member::Member(const char *name, bool pub, std::nullptr_t value) : _name(name), _public(pub), _constant(false)
{
    // create a null member
    _info = new NullMember();
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 */
Member::Member(const char *name, bool pub, int value) : _name(name), _public(pub), _constant(false)
{
    // create a long member
    _info = new LongMember(value);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 */
Member::Member(const char *name, bool pub, long value) : _name(name), _public(pub), _constant(false)
{
    // create a long member
    _info = new LongMember(value);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 */
Member::Member(const char *name, bool pub, bool value) : _name(name), _public(pub), _constant(false)
{
    // create a bool member
    _info = new BoolMember(value);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 */
Member::Member(const char *name, bool pub, char value) : _name(name), _public(pub), _constant(false)
{
    // create a new string member
    _info = new StringMember(&value, 1);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  pub         Is this a public property (otherwise it is protected)
 *  @param  value       The value to add
 */
Member::Member(const char *name, bool pub, const std::string &value) : _name(name), _public(pub), _constant(false)
{
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
Member::Member(const char *name, bool pub, const char *value, int size) : _name(name), _public(pub), _constant(false)
{
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
Member::Member(const char *name, bool pub, double value) : _name(name), _public(pub), _constant(false)
{
    // create a new double member
    _info = new DoubleMember(value);
}

/**
 *  Constructor
 *  @param  name        Name of the method
 *  @param  pub         Is this a public method (otherwise it is protected)
 *  @param  method      The method to add
 */
Member::Member(const char *name, bool pub, const _Method &method, const std::initializer_list<Argument> &arguments) : _name(name), _public(pub), _constant(false)
{
    // create method member
    _info = new MethodMember(name, method, arguments);
}

/**
 *  Copy constructor
 *  @param  member      The member to copy
 */
Member::Member(const Member &member)
{
    // copy info object, and name and public members
    _info = member._info;
    _name = member._name;
    _public = member._public;
    _constant = member._constant;
    
    // update refcount in info object
    _info->refcount(+1);
}

/**
 *  Move constructor
 *  @param  member      The member to move
 */
Member::Member(Member &&member)
{
    // move info object, and name and public properties
    _info = member._info;
    _name = std::move(member._name);
    _public = member._public;
    _constant = member._constant;
    
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
    return _info && _info->isMethod();
}
    
/**
 *  Is this a class constant
 *  @return bool
 */
bool Member::isClassConst()
{
    return _constant;
}

/**
 *  Is this a class constant
 *  @param  bool _constant
 *  @return bool
 */
bool Member::isClassConst(bool _const)
{
    bool oldval = _constant;
    _constant = _const;
    return oldval;
}

/**
 *  Internal method to declare the property
 *  @var zend_class_entry
 */
void Member::declare(struct _zend_class_entry *entry)
{
    if(_constant)
        std::cout << "declareConst(" << _name.c_str() << "):" << std::endl;
    else
        std::cout << "declare(" << _name.c_str() << "):" << std::endl;


    // let the info object handle stuff
    if(_constant)
        _info->declareConst(entry, _name.c_str(), _name.size() TSRMLS_CC);
    else
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
    _info->fill(entry, classname, _public);
}
    
/**
 *  End of namespace
 */
}

