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
    // add a function
    _functions.push_back(std::make_shared<Function>(name, function, arguments));
    
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
    // add a function
    _functions.push_back(std::make_shared<Function>(name, function, arguments));

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
    // add a function
    _functions.push_back(std::make_shared<Function>(name, function, arguments));

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
    // add a function
    _functions.push_back(std::make_shared<Function>(name, function, arguments));

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
void Namespace::apply(const std::function<void(const std::string &ns, Function &func)> &callback)
{
    // loop through the functions, and apply the callback
    for (auto &function : _functions) callback(_name, *function);
    
    // loop through the other namespaces
    for (auto &ns : _namespaces) ns->apply([this, callback](const std::string &ns, Function &func) {
        
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
void Namespace::apply(const std::function<void(const std::string &ns, ClassBase &clss)> &callback)
{
    // loop through the classes, and apply the callback
    for (auto &c : _classes) callback(_name, *c);
    
    // loop through the other namespaces
    for (auto &ns : _namespaces) ns->apply([this, callback](const std::string &ns, ClassBase &clss) {
        
        // if this is the root namespace, we don't have to change the prefix
        if (_name.size() == 0) return callback(ns, clss);
        
        // construct a new prefix
        // @todo this could be slightly inefficient
        return callback(_name + "\\" + ns, clss);
    });
}

/**
 *  Filling ini entries into external zend_ini_entry array
 *  @param  zend_ini_entry*
 */
void Namespace::fill_ini(zend_ini_entry *ini_entries, int module_number)
{
    // loop through the ini entries
    unsigned int Ind = 0;
    for (auto &ini : _ini_entries) ini->fill(&ini_entries[Ind++], module_number);

    // add last empty ini entry (Zend, for some reason, it requires)
    zend_ini_entry empty_entry { 0, 0, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 0, 0, 0, nullptr };
    ini_entries[Ind] = empty_entry;
}

/**
 *  End namespace
 */
}

