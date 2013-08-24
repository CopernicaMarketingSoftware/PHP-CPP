/**
 *  Entry.cpp
 *
 *  The kickstart function that loads the library. When PHP loads an extension,
 *  if runs a dlopen() call to open the .so file, and the it locates the
 *  get_module()' function - which it then calls.
 *
 *  In this file we have implemented this function.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @documentation private
 */
#include "includes.h"

/**
 *  The get_entry function
 *  @return zend_module_entry_t
 */
DLL_EXPORT zend_module_entry_t *get_entry()
{


}

