/**
 *  DelayedFree.h
 *
 *  Sometimes a piece of data must be freed when a function gets out of
 *  scope. In stead of putting the efree() call right before every possible
 *  function end point (exceptions, returns, zend_errors()), we can use
 *  this simple class instead
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class DelayedFree
{
private:
    /**
     *  The data that has to be free'd when the object falls out of scope
     *  @var void*
     */
    void *_data;

public:
    /**
     *  Constructor
     *  @param  data        Data that will be freed on destruction
     */
    DelayedFree(void *data) : _data(data) {}
    
    /**
     *  Destructor
     */
    virtual ~DelayedFree()
    {
        // free the data
        efree(_data);
    }
};

/**
 *  End of namespace
 */
}
