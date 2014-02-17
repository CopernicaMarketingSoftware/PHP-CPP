/**
 *  MemberModifier.h
 *
 *  In this file an enumeration type is with the possible
 *  member modifiers
 *
 *  @author Martijn Otto
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Supported member modifiers
 */
typedef enum _MemberModifier {
    /**
     *  Define whether a member has an implementation
     *  and whether the implementation can be overwritten
     *  in an extending class
     *
     *  These properties are only useful for functions
     */
    abstractMember  =   0x02,
    finalMember     =   0x04,

    /**
     *  Define the access level for a member
     */
    publicMember    =   0x100,
    protectedMember =   0x200,
    privateMember   =   0x400,

    /**
     *  Define a member that cannot be overwritten.
     *  Constant members are always public.
     *
     *  This property is only useful for properties
     */
    constMember     =   0
} MemberModifier;

/**
 *  End namespace
 */
}
