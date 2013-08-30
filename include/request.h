/**
 *  Request.h
 *
 *  During the lifetime of the extension, multiple requests can be handled
 *  by it. For every request that is handled, a request object is created.
 *
 *  The base class for the request is implemented in this file. If you'd like
 *  to add state variables to the request you can override this class and
 *  add the extra features you'd like. If you override this method, you should
 *  also override Extension::request() to return an instance of a different 
 *  class.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
 
/**
 *  Set up namespace
 */
namespace PhpCpp {

/**
 *  Forward definitions
 */
class Extension;

/**
 *  Class definition
 */
class Request
{
public:
    /**
     *  Constructor
     *  @param  extension
     */
    Request(Extension *extension) : _extension(extension) {}
    
    /**
     *  Destructor
     */
    virtual ~Request() {}
    
    /**
     *  Initialize the request
     * 
     *  This method is called directly after the object was destructed. You can
     *  override this method to add your own initialization code.
     * 
     *  @return bool
     */
    virtual bool initialize()
    {
        return true;
    }
    
    /**
     *  Finalize the request
     * 
     *  This method is called right before the object is destructed. Note that
     *  the object is going to be destructed anyway, even if this method returns
     *  false
     * 
     *  @return bool
     */
    virtual bool finalize()
    {
        return true;
    }

protected:
    /**
     *  The extension that this request belongs to
     *  @var Extension*
     */
    Extension *_extension;
    
    /**
     *  Optional extra data
     *	@var Type
     */
    Type _data;
};

/**
 *  End of namespace
 */
}

