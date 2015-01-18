/**
 *  Namespace.cpp
 *
 *  Implementation of the namespace class
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Open namespace
 */
namespace Php {

/**
 *  Add a native function directly to the extension
 *  @param  name        Name of the function
 *  @param  function    The function to add
 *  @param  arguments   Optional argument specification
 *  @return Namespace   Same object to allow chaining
 */
Namespace &Namespace::add(const char *name, const native_callback_0 &function, const Arguments &arguments)
{
    // skip when locked
    if (locked()) return *this;

    // add a function
    _functions.push_back(std::make_shared<NativeFunction>(name, function, arguments));
    
    // allow chaining
    return *this;
}

/**
 *  Add a native function directly to the extension
 *  @param  name        Name of the function
 *  @param  function    The function to add
 *  @param  arguments   Optional argument specification
 *  @return Namespace   Same object to allow chaining
 */
Namespace &Namespace::add(const char *name, const native_callback_1 &function, const Arguments &arguments)
{
    // skip when locked
    if (locked()) return *this;

    // add a function
    _functions.push_back(std::make_shared<NativeFunction>(name, function, arguments));

    // allow chaining
    return *this;
}

/**
 *  Add a native function directly to the extension
 *  @param  name        Name of the function
 *  @param  function    The function to add
 *  @param  arguments   Optional argument specification
 *  @return Namespace   Same object to allow chaining
 */
Namespace &Namespace::add(const char *name, const native_callback_2 &function, const Arguments &arguments)
{
    // skip when locked
    if (locked()) return *this;

    // add a function
    _functions.push_back(std::make_shared<NativeFunction>(name, function, arguments));

    // allow chaining
    return *this;
}

/**
 *  Add a native function directly to the extension
 *  @param  name        Name of the function
 *  @param  function    The function to add
 *  @param  arguments   Optional argument specification
 *  @return Namespace   Same object to allow chaining
 */
Namespace &Namespace::add(const char *name, const native_callback_3 &function, const Arguments &arguments)
{
    // skip when locked
    if (locked()) return *this;

    // add a function
    _functions.push_back(std::make_shared<NativeFunction>(name, function, arguments));

    // allow chaining
    return *this;
}

/**
 *  Apply a callback to each registered function
 * 
 *  The callback will be called with the name of the namespace, and
 *  a reference to the registered function.
 * 
 *  @param  callback
 */
void Namespace::functions(const std::function<void(const std::string &ns, NativeFunction &func)> &callback)
{
    // loop through the functions, and apply the callback
    for (auto &function : _functions) callback(_name, *function);
    
    // loop through the other namespaces
    for (auto &ns : _namespaces) ns->functions([this, callback](const std::string &ns, NativeFunction &func) {
        
        // if this is the root namespace, we don't have to change the prefix
        if (_name.size() == 0) return callback(ns, func);
        
        // construct a new prefix
        // @todo this could be slightly inefficient
        return callback(_name + "\\" + ns, func);
    });
}

/**
 *  Apply a callback to each registered class
 * 
 *  The callback will be called with the name of the namespace, and
 *  a reference to the registered class.
 * 
 *  @param  callback
 */
void Namespace::classes(const std::function<void(const std::string &ns, ClassBase &clss)> &callback)
{
    // loop through the classes, and apply the callback
    for (auto &c : _classes) callback(_name, *c);
    
    // loop through the other namespaces
    for (auto &ns : _namespaces) ns->classes([this, callback](const std::string &ns, ClassBase &clss) {
        
        // if this is the root namespace, we don't have to change the prefix
        if (_name.size() == 0) return callback(ns, clss);
        
        // construct a new prefix
        // @todo this could be slightly inefficient
        return callback(_name + "\\" + ns, clss);
    });
}

/**
 *  Apply a callback to each registered constant
 * 
 *  The callback will be called with the name of the namespace, and
 *  a reference to the registered constant
 * 
 *  @param  callback
 */
void Namespace::constants(const std::function<void(const std::string &ns, Constant &constant)> &callback)
{
    // loop through the constants, and apply the callback
    for (auto &c : _constants) callback(_name, *c);
    
    // loop through the other namespaces
    for (auto &ns : _namespaces) ns->constants([this, callback](const std::string &ns, Constant &constant) {
        
        // if this is the root namespace, we don't have to change the prefix
        if (_name.size() == 0) return callback(ns, constant);
        
        // construct a new prefix
        // @todo this could be slightly inefficient
        return callback(_name + "\\" + ns, constant);
    });
}

/**
 *  End namespace
 */
}

