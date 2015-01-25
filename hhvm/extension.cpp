/**
 *  Extension.cpp
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013, 2014, 2015 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Constructor that defines a number of functions right away
 *  @param  name        Extension name
 *  @param  version     Extension version string
 *  @param  apiversion  The API version number
 */
Extension::Extension(const char *name, const char *version, int apiversion) :
    Namespace(""), _impl(new ExtensionImpl(this, name, version, apiversion)) {}
    
/**
 *  Destructor
 */
Extension::~Extension()
{
    // get rid of the implementation object
    delete _impl;
}

/**
 *  Register a function to be called when the PHP engine is ready
 *  @param  callback
 *  @return Extension
 */
Extension &Extension::onStartup(const Callback &callback)
{
    // pass on to the implementation
    _impl->onStartup(callback);
    
    // allow chaining
    return *this;
}

/**
 *  Register a function to be called when the PHP engine is going to stop
 *  @param  callback
 *  @return Extension
 */
Extension &Extension::onShutdown(const Callback &callback)
{
    // pass on to the implementation
    _impl->onShutdown(callback);
    
    // allow chaining
    return *this;
}

/**
 *  Register a callback that is called at the beginning of each pageview/request
 *  @param  callback
 */
Extension &Extension::onRequest(const Callback &callback)
{
    // pass on to the implementation
    _impl->onRequest(callback);
    
    // allow chaining
    return *this;
}

/**
 *  Register a callback that is called to cleanup things after a pageview/request
 *  @param  callback
 */
Extension &Extension::onIdle(const Callback &callback)
{
    // pass on to the implementation
    _impl->onIdle(callback);
    
    // allow chaining
    return *this;
}

/**
 *  Retrieve the module pointer
 * 
 *  This is the memory address that should be exported by the get_module()
 *  function.
 *
 *  @return void*
 */
void *Extension::module()
{
    // pass on to the implementation
    return _impl->module();
}

/**
 *  End of namespace
 */
}

