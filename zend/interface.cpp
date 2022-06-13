/**
 *  Interface.cpp
 * 
 *  Implementation file for the Interface class
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2022 Copernica BV
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Begin of namespace
 */
namespace Php {

/**
 *  Constructor
 *  @param  name
 */
Interface::Interface(const char *name) : ClassBase(name, ClassType::Interface) {}

/**
 *  End of namespace
 */
}

