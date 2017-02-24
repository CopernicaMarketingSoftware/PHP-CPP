/**
 *  @file classtype.h
 *
 *  Internal class types enumeration.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Enumeration definition.
 *
 *  The PHP-CPP library tries to hide the Zend engine internals completely from
 *  the user. Therefore, it does not include any of the Zend header files, nor
 *  can it refer to the constants defined in the Zend header files. The
 *  following constants have been copied from Zend. If the Zend engine ever
 *  changes (which we do not expect) we should also copy the constant values
 *  used here.
 *
 */
enum class ClassType {
    Regular     =   0x00,
    Abstract    =   0x20,
    Final       =   0x04,
    Interface   =   0x40,
};

/**
 *  End namespace
 */
}
