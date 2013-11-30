class Exception;

namespace Php
{

class TestException : public Exception
{
public:
	TestException();
	TestException(const std::string &message);
	virtual std::string GetExportName();
};

}
