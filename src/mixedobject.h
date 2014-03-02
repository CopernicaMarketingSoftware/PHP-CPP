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
    /**
     *  The actual object is the first member, so that casting
     *  the MixedObject to a zend_object will also result in a valid pointer
     *  @var    zend_object
     */
    zend_object php;
    
    /**
     *  Pointer to the C++ implementation
     *  @var    Base
     */
    Base *cpp;
};

/**
 *  End of namespace
 */
}

 
