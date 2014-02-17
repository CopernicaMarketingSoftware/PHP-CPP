/**
 *  ClassModifier.h
 *
 *  In this file an enumeration type is defined with
 *  the possible class modifiers.
 *
 *  @author Martijn Otto
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Supported class modifiers
 */
typedef enum _ClassModifier {
    regular     =   0x00,
    abstract    =   0x20,
    final       =   0x40,
    interface   =   0x80
} ClassModifier;

/**
 *  End of namespace
 */
}
