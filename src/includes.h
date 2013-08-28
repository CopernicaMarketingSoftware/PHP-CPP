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

// for debugging
#include <iostream>

/**
 *  PHP includes
 */
#include "php.h"

/**
 *  Macro to convert results to success status
 */
#define BOOL2SUCCESS(b) ((b) ? SUCCESS : FAILURE)

/**
 *  Include other files from this library
 */
#include "../include/type.h"
#include "../include/request.h"
#include "../include/argument.h"
#include "../include/value.h"
#include "../include/function.h"
#include "../include/extension.h"

/**
 *  Interface files for internal use only
 */
#include "callable.h"
#include "arginfo.h"
#include "functions.h"

