/**
 *  Modifiers.cpp
 *
 *  In this file an enumeration type is with the possible
 *  member modifiers
 *
 *  @author Martijn Otto <martijn.otto@copernica.com>
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 * 
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  The modifiers are constants
 */
const int Static    =   0x01;
const int Abstract  =   0x02;
const int Final     =   0x04;
const int Public    =   0x100;
const int Protected =   0x200;
const int Private   =   0x400;
const int Const     =   0;

/**
 *  Modifiers that are supported for methods and properties
 */
const int MethodModifiers           =   Final | Public | Protected | Private | Static;
const int PropertyModifiers         =   Final | Public | Protected | Private | Const | Static;

/**
 *  End namespace
 */
}
