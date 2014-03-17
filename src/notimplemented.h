/**
 *  NotImplemented.h
 *
 *  Exception that is thrown and catched by the library internally to detect
 *  if a magic method was implemented or not.
 *
 *  Classes have magic methods (like __unset, __isset, etcetera). These methods
 *  can be implemented by the extension writer, but they do not have to be.
 *
 *  The default implementation of the methods _could_ be to pass on the method
 *  to the original Zend engine, but the problem is that the magic methods from
 *  the PHP-CPP library do not have the same signature as the functions in the
 *  Zend engine. Passing them on directly is thus not possible. 
 *
 *  For that reason, the default implementation throw an exception that is
 *  immediately caught by the PHP-CPP library, so that it knows that the user
 *  has not overridden the methods, and the default Zend engine magic method
 *  can be called instead
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class NotImplemented : public std::exception
{
public:
    /**
     *  Constructor
     */
    NotImplemented() : std::exception() {}
    
    /**
     *  Destructor
     */
    virtual ~NotImplemented() throw() {}

};

/**
 *  End namespace
 */
}

