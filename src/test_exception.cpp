#include "includes.h"

namespace Php
{

TestException::TestException() :
	Exception()
{

}

TestException::TestException(const std::string &message) :
	Exception(message)
{

}

std::string TestException::GetExportName()
{
	return std::string("TestException");
}

}
