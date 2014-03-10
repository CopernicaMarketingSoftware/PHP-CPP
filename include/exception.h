/**
 *  Exception.h
 *  Implementation of Php Exceptions.
 * 
 *  @author Jasper van Eck <jasper.vaneck@copernica.com>
 *  @copyright 2013, 2014 Copernica BV
 */
#include <exception>

/**
 *  Set up namespace
 */
namespace Php {

/** 
 *  Class definition
 */
class Exception : public std::exception
{
private:
    /**
     *  The exception message
     *  @var    char*
     */
    std::string _message;
    
    /**
     *  The PHP exception code
     *  @var    int
     */
    int _code;
    
public:
    /**
     *  Constructor
     *  @param  &string
     */
    Exception(const std::string &message, int code = 0) : std::exception(), _message(message), _code(code)
    {
    }
    
    /**
     *  Destructor
     */
    virtual ~Exception() throw()
    {
    }
    
    /**
     *  Returns the message of the exception.
     *  @return &string
     */
    std::string &message() throw()
    {
        return _message;
    }

    /**
     *  Process the exception
     * 
     *  This method is called only from withing the PHP-CPP library,
     *  and will turn the exception into a PHP exception
     * 
     *  @internal
     */
    virtual void process();
};

/**
 *  End of namespace
 */
}

 
