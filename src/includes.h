/**
 *  Includes.h
 *
 *  Startup include file to compile the phpcpp library
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Include standard C and C++ libraries
 */
#include <stdlib.h>
#include <string>
#include <initializer_list>
#include <vector>
#include <map>
#include <memory>
#include <set>

// for debugging
#include <iostream>

/**
 *  PHP includes
 */
#include <php.h>

/**
 *  Macro to convert results to success status
 */
#define BOOL2SUCCESS(b) ((b) ? SUCCESS : FAILURE)

/**
 *  Include other files from this library
 */
#include "../include/type.h"
#include "../include/value.h"
#include "../include/hiddenpointer.h"
#include "../include/environment.h"
#include "../include/argument.h"
#include "../include/byval.h"
#include "../include/byref.h"
#include "../include/global.h"
#include "../include/member.h"
#include "../include/parameters.h"
#include "../include/function.h"
#include "../include/base.h"
#include "../include/class.h"
#include "../include/classinfo.h"
#include "../include/extension.h"
#include "../include/globals.h"

/**
 *  Interface files for internal use only
 */
#include "nativefunction.h"
#include "internalfunction.h"

