/**
 *  OrigException.h
 *
 *  Class that wraps around an exception that was thrown by PHP code,
 *  and that could - but not necessarily has to - be caught by C++
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

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
    bool _restored = false;
    
public:
    /**
     *  Constructor
     *  @param  zval
     */
    OrigException(struct _zval_struct *zval) : 
        Value(zval), Exception("OrigException") {}
    
    /**
     *  Copy constructor
     *  @param  exception
     */
    OrigException(const OrigException &exception) : 
        Value(exception), Exception("OrigException"), _restored(exception._restored) {}
    
    /**
     *  Move constructor
     *  @param  exception
     */
    OrigException(OrigException &&exception) :
        Value(std::move(exception)), Exception("OrigException"), _restored(exception._restored) 
    {
        // set other exception to restored so that it wont 
        // do anything on destruction
        exception._restored = true;
    }
    
    /**
     *  Destructor
     */
    virtual ~OrigException() throw();
    
    /**
     *  Process the exception
     * 
     *  This will restore the exception so that it can be further processed 
     *  in PHP code
     *  @internal
     */
    virtual void process() override
    {
        // mark exception as restored
        _restored = true;
    }
};

/**
 *  End of namespace
 */
}
