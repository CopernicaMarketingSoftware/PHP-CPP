/**
 *	Functions.cpp
 *
 *	Implementation for the functions class
 *
 *	@author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *	@copyright 2013 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace PhpCpp {

/**
 *  Constructor
 *  @param  functions   The functions to parse
 */
Functions::Functions(const std::initializer_list<Function> &functions) : _functions(functions)
{
	// allocate the function entries
	_entries = new zend_function_entry[functions.size() + 1];
	
	// keep iterator counter
	int i = 0;
	
	// loop through the functions
	for (auto it = begin(functions); it != functions.end(); it++)
	{
		// let the callable fill the array
		it->internal()->fill(&_entries[i++]);
	}
	
	// last entry should be set to all zeros
	zend_function_entry *last = &_entries[i];
	
	// all should be set to zero
	memset(last, 0, sizeof(zend_function_entry));
}

/**
 *  Destructor
 */
Functions::~Functions()
{
	delete[] _entries;
}

/**
 *	End of namespace
 */
}

