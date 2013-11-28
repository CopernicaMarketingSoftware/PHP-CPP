/**
 *  error.h
 *
 *  Contains 'static' function that can be used to report errors. For now throwing an Exception (zend default)
 *  is enough. In the future, we might want to implement extensive exception support.
 *
 *  @author Swen Kooij <photonios@outlook.com>
 *  @copyright 2013 Swen Kooij
 */

/**
 * Setup namespace
 */
namespace Php
{
	/**
	 * Throws an Exception (Zend Default) with the specified message.
	 * @param error_msg A string, describing the reason of failure.
	 */
	void ThrowError(const std::string &error_msg);
}
