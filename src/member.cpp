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
 *  Internal method to declare the property
 *  @var zend_class_entry
 */
void Member::declare(struct _zend_class_entry *entry)
{
    std::cout << "declare property " << _name << std::endl;
    
    // declare the property
    zend_declare_property(entry, _name.c_str(), _name.size(), _value._val, _public ? ZEND_ACC_PUBLIC : ZEND_ACC_PROTECTED TSRMLS_CC);
}
    
/**
 *  End of namespace
 */
}

