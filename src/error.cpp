/**
 *  error.cpp
 *
 *  @author Swen Kooij <photonios@outlook.com>
 *  @copyright 2013 Swen Kooij
 */

#include "includes.h"

/**
 * Setup namespace
 */
namespace Php
{
	void ThrowError(const std::string &error_msg)
	{
		char *error_msg_writeable = new char[error_msg.size() + 1];
		std::copy(error_msg.begin(), error_msg.end(), error_msg_writeable);
		error_msg_writeable[error_msg.size()] = '\0';

		zend_throw_exception(zend_exception_get_default(), error_msg_writeable, 0 TSRMLS_CC);
	}

} // namespace Php
