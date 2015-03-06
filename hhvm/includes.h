/**
 *  Includes.h
 * 
 *  All includes for compiling the HHVM implementation of PHP-CPP
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Standard C and C++ libraries
 */
#include <functional>
#include <list>
#include <memory>
#include <vector>
#include <map>
#include <string.h>
#include <iostream>
#include <set>

/**
 *  HHVM includes
 */


/**
 *  Public include files
 */
#include "../include/version.h"
#include "../include/type.h"
#include "../include/hashparent.h"
#include "../include/ini.h"
#include "../include/inivalue.h"
#include "../include/value.h"
#include "../include/parameters.h"
#include "../include/classtype.h"
#include "../include/argument.h"
#include "../include/modifiers.h"
#include "../include/classbase.h"
#include "../include/interface.h"
#include "../include/iterator.h"
#include "../include/traversable.h"
#include "../include/serializable.h"
#include "../include/class.h"
#include "../include/namespace.h"
#include "../include/extension.h"

/**
 *  Generic implementation header files
 */
#include "../common/extensionbase.h"
#include "../common/streambuf.h"

/**
 *  Specific HHVM header files for the implementation only
 */
#include "extensionimpl.h"

