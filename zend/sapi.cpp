/**
 *  Sapi.cpp
 *
 *  This file holds the implementation for the Php::sapi_name() function
 *
 *  @author Toon Schoenmakers <toon.schoenmakers@copernica.com>
 */

/**
 *  Dependencies
 */
#include "includes.h"

/**
 *  Open PHP namespace
 */
namespace Php {

/**
 *  Retrieve the sapi name we're running on
 *  @return  const char*
 */
const char *sapi_name()
{
    return sapi_module.name;
}

/**
 *  End of namespace
 */
}
