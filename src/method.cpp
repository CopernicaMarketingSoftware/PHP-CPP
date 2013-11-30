/**
 *  Method.cpp
 */

#include "includes.h"

namespace Php
{
Value _Method::invoke(Environment &environment, Parameters &parameters)
{
	 // the object to call a method on
	Base *base = parameters.object();

	// Wrap this in a try catch and catch any exception, we'll rethrow it
	// as a PHP exception
	try
	{
		// find out which method to call, and call it
		switch (_type) {
		case 0:     (base->*_callback.m0)(); return Value();
		case 1:     (base->*_callback.m1)(parameters); return Value();
		case 2:     (base->*_callback.m2)(environment); return Value();
		case 3:     (base->*_callback.m3)(environment, parameters); return Value();
		case 4:     return (base->*_callback.m4)();
		case 5:     return (base->*_callback.m5)(parameters);
		case 6:     return (base->*_callback.m6)(environment);
		case 7:     return (base->*_callback.m7)(environment, parameters);
		default:    return Value();
		}
	}
	catch(Exception &excp)
	{
		// Copy the error message into a write able string literal
		std::string error_msg = excp.GetMessage();
		char *error_msg_writeable = new char[error_msg.size() + 1];
		std::copy(error_msg.begin(), error_msg.end(), error_msg_writeable);
		error_msg_writeable[error_msg.size()] = '\0';

		// If the thrown exception is a Php::Exception (base class) , then just throw
		// the base exception class
		if(excp.GetExportName() == Php::Constants::ExceptionClassName)
		{
			zend_throw_exception(zend_exception_get_default(), error_msg_writeable, 0 TSRMLS_CC);
		}
		else
		{
			// Find the exception class to be thrown
			_ClassInfo *excp_class = environment._extension->FindClassByName(excp.GetExportName());

			// If we found it, throw that one, otherwise throw a base exception
			if(excp_class != NULL)
			{
				struct _zend_class_entry *excp_class_entry = excp_class->_entry;
				zend_throw_exception(excp_class_entry, error_msg_writeable, 0 TSRMLS_CC);
			}
			else
			{
				zend_throw_exception(zend_exception_get_default(), error_msg_writeable, 0 TSRMLS_CC);
			}
		}

		// We can just return an empty value here
		return Value();
	}
}
} //namespace Php
