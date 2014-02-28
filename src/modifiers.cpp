/**
 *  Modifiers.cpp
 *
 *  In this file an enumeration type is with the possible
 *  member modifiers
 *
 *  @author Martijn Otto
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
const int Abstract  =   0x02;
const int Final     =   0x04;
const int Public    =   0x100;
const int Protected =   0x200;
const int Private   =   0x400;
const int Const     =   0;

/**
 *  End namespace
 */
}
