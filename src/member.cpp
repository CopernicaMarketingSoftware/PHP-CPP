/**
 *  Member.cpp
 *
 *  Implementation for class members
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 * 
 * 
 *  @todo   refactor this class to a property
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

#ifdef XXXX

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  flags       Flag access to a class member (bublic, protected etc)
 */
Member::Member(const char *name, MemberModifier flags) : _name(name), _flags(flags)
{
    // create a null member
    _info = new NullMember();
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  flags       Flag access to a class member (bublic, protected etc)
 *  @param  value       The value to add
 */
Member::Member(const char *name, MemberModifier flags, std::nullptr_t value) : _name(name), _flags(flags)
{
    // create a null member
    _info = new NullMember();
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  flags       Flag access to a class member (bublic, protected etc)
 *  @param  value       The value to add
 */
Member::Member(const char *name, MemberModifier flags, int value) : _name(name), _flags(flags)
{
    // create a long member
    _info = new LongMember(value);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  flags       Flag access to a class member (bublic, protected etc)
 *  @param  value       The value to add
 */
Member::Member(const char *name, MemberModifier flags, long value) : _name(name), _flags(flags)
{
    // create a long member
    _info = new LongMember(value);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  flags       Flag access to a class member (bublic, protected etc)
 *  @param  value       The value to add
 */
Member::Member(const char *name, MemberModifier flags, bool value) : _name(name), _flags(flags)
{
    // create a bool member
    _info = new BoolMember(value);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  flags       Flag access to a class member (bublic, protected etc)
 *  @param  value       The value to add
 */
Member::Member(const char *name, MemberModifier flags, char value) : _name(name), _flags(flags)
{
    // create a new string member
    _info = new StringMember(&value, 1);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  flags       Flag access to a class member (bublic, protected etc)
 *  @param  value       The value to add
 */
Member::Member(const char *name, MemberModifier flags, const std::string &value) : _name(name), _flags(flags)
{
    // create a new string member
    _info = new StringMember(value);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  flags       Flag access to a class member (bublic, protected etc)
 *  @param  value       The value to add
 *  @param  size        String length
 */
Member::Member(const char *name, MemberModifier flags, const char *value, int size) : _name(name), _flags(flags)
{
    // create a new string member
    if (size < 0) size = strlen(value);
    _info = new StringMember(value, size);
}

/**
 *  Constructor
 *  @param  name        Name of the member
 *  @param  flags       Flag access to a class member (bublic, protected etc)
 *  @param  value       The value to add
 */
Member::Member(const char *name, MemberModifier flags, double value) : _name(name), _flags(flags)
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
Member::Member(const char *name, MemberModifier flags, const _Method &method, const std::initializer_list<Argument> &arguments) : _name(name), _flags(flags)
{
    // methods cannot be constant, so in that case we default to
    // a public member function instead
    if (_flags == constMember) _flags = publicMember;

    // create method member
    _info = new MethodMember(name, method, arguments);
}

/**
 *  Copy constructor
 *  @param  member      The member to copy
 */
Member::Member(const Member &member) : _flags(member._flags)
{
    // copy info object, and name and public members
    _info = member._info;
    _name = member._name;

    // update refcount in info object
    _info->refcount(+1);
}

/**
 *  Move constructor
 *  @param  member      The member to move
 */
Member::Member(Member &&member) : _flags(member._flags)
{
    // move info object, and name and public properties
    _info = member._info;
    _name = std::move(member._name);

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
 *  Internal method to declare the property
 *  @var zend_class_entry
 */
void Member::declare(struct _zend_class_entry *entry)
{
    // let the info object handle stuff
    _info->declare(entry, _name.c_str(), _name.size(), _flags TSRMLS_CC);
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
    _info->fill(entry, classname, _flags);
}
    
#endif 

/**
 *  End of namespace
 */
}

