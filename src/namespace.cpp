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
 *  Initialize all functions in this namespace
 *  @param  parent      Namespace prefix of the parent
 *  @param  entries     The array to be filled
 *  @return int         Number of functions that were initialized
 */
size_t Namespace::initialize(const std::string &parent, struct _zend_function_entry entries[])
{
    // keep iterator counter
    int count = 0;
    
    // the namespace to use
    std::string prefix = parent.size() ? parent + "\\" + _name : _name;

    // loop through the functions
    for (auto &function : _functions)
    {
        // retrieve entry
        zend_function_entry *entry = &entries[count++];

        // let the function fill the entry
        function->initialize(prefix, entry);
    }
    
    // loop through the namespace
    for (auto &ns : _namespaces)
    {
        // let the namespace initialize
        count += ns->initialize(prefix, &entries[count]);
    }
    
    // done
    return count;
}

/**
 *  Initialize the namespace after it was registered
 *  @param  parent      Parent namespace
 *  @param  tsrm_ls
 */
void Namespace::initialize(const std::string &parent TSRMLS_DC)
{
    // the namespace to use
    std::string prefix = parent.size() ? parent + "\\" + _name : _name;
    
    // loop through the classes in this namespace
    for (auto &c : _classes) c->initialize(prefix TSRMLS_CC);
    
    // and loop through the other namespaces
    for (auto &n : _namespaces) n->initialize(prefix TSRMLS_CC);
}


/**
 *  End namespace
 */
}

