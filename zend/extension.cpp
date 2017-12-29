/**
 *  Extension.cpp
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013, 2014 Copernica BV
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
 *  @param  apiversion  API version number
 */
Extension::Extension(const char *name, const char *version, int apiversion) :
    Namespace(""), _impl(new ExtensionImpl(this, name, version, apiversion)) {}
    
/**
 *  Destructor
 */
Extension::~Extension() = default;

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
 *  Is the extension object in a locked state? This happens after the
 *  get_module() function was called for the first time ("apache reload"
 *  forces a new call to get_module())
 * 
 *  @return bool
 */
bool Extension::locked() const
{
    return _impl->locked();
}

/**
 *  Add a ini entry to the extension by moving it
 *  @param  ini         The php.ini setting
 *  @return Extension   Same object to allow chaining
 */
Extension &Extension::add(Ini &&ini)
{
    // pass on to the implementation
    _impl->add(std::move(ini));
    
    // allow chaining
    return *this;
}

/**
 *  Add a ini entry to the extension by copying it
 *  @param  ini         The php.ini setting
 *  @param  Extension   Same object to allow chaining
 */
Extension &Extension::add(const Ini &ini)
{
    // pass on to the implementation
    _impl->add(ini);
    
    // allow chaining
    return *this;
}

/**
 *  The total number of php.ini variables
 *  @return size_t
 */
size_t Extension::iniVariables() const
{
    // pass on to the implementation
    return _impl->iniVariables();
}

/**
 *  Apply a callback to each php.ini variable
 *
 *  The callback will be called with a reference to the ini variable.
 *
 *  @param  callback
 */
void Extension::iniVariables(const std::function<void(Ini &ini)> &callback)
{
    // pass on to the implementation
    _impl->iniVariables(callback);
}

/**
 *  End of namespace
 */
}

