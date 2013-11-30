/**
 *  exception.cpp
 *
 *  Represents an exception that can be thrown from C++, but also represents exceptions that
 *  can be thrown from PHP, it's the PHP-CPP exception base class.
 *
 *  @author Swen Kooij (Photonios) <photonios@outlook.com>
 *  @copyright 2013 Swen Kooij (Photonios)
 */

#include "includes.h"

namespace Php
{

Exception::Exception() :
	m_message ("")
{

}

Exception::Exception(const std::string &message) :
	m_message (message)
{

}

void Exception::__construct(Php::Parameters &params)
{
	if(params.empty())
		return;

	if(!params[0].isString())
		return;

	m_message = params[0].stringValue();
}

std::string Exception::GetMessage()
{
	return m_message;
}

const char * Exception::what() const noexcept
{
	return m_message.c_str();
}

std::string Exception::GetExportName()
{
	return "PhpCppException";
}

} // namespace Php
