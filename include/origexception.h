/**
 *  OrigException.h
 *
 *  Class that wraps around an exception that was thrown by PHP code,
 *  and that could - but not necessarily has to - be caught by C++
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

void cpp_throw_exception_hook(struct _zval_struct *exception);

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class OrigException : public Value, public Exception
{
private:    
    /**
     *  Has the exception been restored by the C++ code so that it can be dealt by PHP?
     *  @var    boolean
     */
    bool _restored;
    
public:
    /**
     *  Constructor
     *  @param  zval
     */
    OrigException(struct _zval_struct *zval);
    
    /**
     *  Copy constructor
     *  @param  exception
     */
    OrigException(const OrigException &exception);
    
    /**
     *  Move constructor
     *  @param  exception
     */
    OrigException(OrigException &&exception);
    
    /**
     *  Destructor
     */
    virtual ~OrigException() throw();
    
    /**
     *  Restore the exception - because the exception was not handled by C++ code
     *  @internal
     */
    void restore();

    static void hook();
};

/**
 *  End of namespace
 */
}
