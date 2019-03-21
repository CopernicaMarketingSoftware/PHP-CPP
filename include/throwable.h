/**
 *  Throwable.h
 *
 *  Base class for exceptions and errors, where an error is a runtime
 *  problem with the program (like file-does-not-exist), and an error
 *  is a more fatal programming problem (like call-to-private-method).
 *
 *  @author Jasper van Eck <jasper.vaneck@copernica.com>
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 - 2019 Copernica BV
 */
#include <exception>
#include <string>

/**
 *  Forward declarations
 */
struct _zend_object;

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT Throwable : public std::runtime_error
{
protected:
    /**
     *  The exception code
     *  @var long int
     */
    long int _code = -1;
    

protected:
    /**
     *  Protected constructor - only derived classes can instantiate
     *  @param  message     The exception message
     */
    Throwable(const std::string &message) : std::runtime_error(message) {}
    
    /**
     *  Another protected constructor
     *  @param  object
     */
    Throwable(struct _zend_object *object);

public:
    /**
     *  Destructor
     */
    virtual ~Throwable() = default;
    
    /**
     *  Rethrow the exception / make sure that it ends up in PHP space
     */
    virtual void rethrow() = 0;
     
    /**
     *  Returns the exception code
     *  @return The exception code
     */
    long int code() const _NOEXCEPT
    {
        // expose the code
        return _code;
    }
};

/**
 *  End of namespace
 */
}
