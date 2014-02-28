/**
 *  Members.cpp
 *
 *  Implementation of the members class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

///**
// *  Destructor
// */
//Members::~Members()
//{
//    // check if there are methods
//    if (_methods) delete[] _methods;
//}
//
///**
// *  Number of methods
// *  @return integer
// */
//int Members::methods()
//{
//    // result variable
//    int result = 0;
//
//    // loop through the functions
//    for (auto it = begin(); it != end(); it++)
//    {
//        // check if this is a method
//        if (it->isMethod()) result++;
//    }
//    
//    // done
//    return result;
//}
//
///**
// *  Get access to the methods
// *  @return Methods
// */
//struct _zend_function_entry *Members::methods(const char *classname)
//{
//    // already set?
//    if (_methods) return _methods;
//    
//    // the number of methods
//    int count = methods();
//    
//    // allocate memory for the functions
//    _methods = new zend_function_entry[count + 1];
//    
//    // keep iterator counter
//    int i = 0;
//
//    // loop through the functions
//    for (auto it = begin(); it != end(); it++)
//    {
//        // skip if this is not a method
//        if (!it->isMethod()) continue;
//        
//        // retrieve entry
//        zend_function_entry *entry = &_methods[i++];
//
//        // let the function fill the entry
//        it->initialize(classname, entry);
//    }
//
//    // last entry should be set to all zeros
//    zend_function_entry *last = &_methods[i];
//
//    // all should be set to zero
//    memset(last, 0, sizeof(zend_function_entry));
//
//    // done
//    return _methods;
//}
//
/**
 *  End of namespace
 */
}

