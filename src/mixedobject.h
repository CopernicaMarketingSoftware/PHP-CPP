/**
 *  MixedObject.h
 *
 *  Structure that combines a Zend object with an object in C++
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Structure that combines a C++ object with a zend object
 */
struct MixedObject
{
    zend_object php;
    Base *cpp;
};

/**
 *  End of namespace
 */
}

 
