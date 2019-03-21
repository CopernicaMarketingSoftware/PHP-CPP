/**
 *  Exception.h
 *
 *  Class for runtime errors. In your C++ extension you can both throw
 *  and catch instances of these class. If you throw an exception, it 
 *  will bubble up to PHP space, where the PHP script can then catch it.
 *  
 *  Exceptions called from PHP space can also be caught in C++ code.
 *  If you put your call to a PHP user space function inside a try-catch
 *  block, you can catch the exceptions as Php::Exception objects.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2019 Copernica BV
 */

/**
 *  Begin of namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT Exception : public Throwable 
{
protected:
    /**
     *  Internal constructor to wrap around an exception object
     *  @param  object
     */
    Exception(struct _zend_object *object) : Throwable(object) {}

    /**
     *  Rethrow the exception / make sure that it ends up in PHP space
     */
    virtual void rethrow() override;

public:
    /**
     *  Public constructor
     *  @param  message
     */
    Exception(const std::string &message) : Throwable(message) {}

    /**
     *  Destructor
     */
    virtual ~Exception() = default;
};

/**
 *  End of namespace
 */
}

