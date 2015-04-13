/**
 *  Eval.cpp
 *
 *  This file holds the implementation for the Php::eval() function
 * 
 *  @author andot <https://github.com/andot>
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
 *  Evaluate a PHP string
 *  @param  phpCode     The PHP code to evaluate
 *  @return Value       The result of the evaluation
 */
Value eval(const char *phpCode) 
{
    // we have a script for this
    return Script(phpCode).execute();
}

/**
 *  Include a file
 *  @param  filename
 *  @return Value
 */
Value include(const char *filename)
{
    // we can simply execute a file
    return File(filename).execute();
}

/**
 *  Include a file only once
 *  @param  filename
 *  @return Value
 */
Value include_once(const char *filename)
{
    // we can simply execute a file
    return File(filename).once();
}

/**
 *  Require a file
 *  This causes a fatal error if the file does not exist
 *  @param  filename
 *  @return Value
 */
Value require(const char *filename)
{
    // create the file
    File file(filename);
    
    // execute if it exists
    if (file.exists()) return file.execute();
    
    // trigger fatal error
    error << filename << " does not exist" << std::flush;
    
    // unreachable
    return nullptr;
}

/**
 *  Require a file only once
 *  This causes a fatal error if the file does not exist
 *  @param  filename
 *  @return Value
 */
Value require_once(const char *filename)
{
    // create the file
    File file(filename);
    
    // execute if it exists
    if (file.exists()) return file.once();
    
    // trigger fatal error
    error << filename << " does not exist" << std::flush;
    
    // unreachable
    return nullptr;
}

/**
 *  Implementation of the dl() function - activate a different PHP extension
 * 
 *  If you open an extension persistently, the static variables inside it are
 *  kept open for as long as apache runs.
 * 
 *  @param  filename
 *  @param  persistent
 */
bool dl(const char *filename, bool persistent)
{
    // create the module
    Module module(filename, persistent);
    
    // start the module
    return module.start();
}

/**
 *  End of namespace
 */
}
