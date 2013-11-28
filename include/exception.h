/**
 * 	Exception.h
 * 	Implementation of Php Exceptions.
 * 
 * 	@author Jasper van Eck <jasper.vaneck@copernica.com>
 * 	@copyright 2013 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/** 
 *  Class definition
 */
class Exception
{
private:
	/**
	 * 	The exception message
	 * 	@var char*
	 */
	char* _message;
	
public:
	/**
	 * 	Constructor
	 * 	@param	string		The exception message.
	 */
	Exception(char* message) throw()
	{
		_message = message;
	}

	/**
	 * 	Destructor
	 */
	~Exception() throw()
	{
	}
	
	/**
	 * 	Returns the message of the exception.
	 * 	@return std::string
	 */
	char* getMessage() const throw()
	{
		return _message;
	}

};

}
