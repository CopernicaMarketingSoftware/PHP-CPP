/**
 *  Environment.h
 *
 *  During the lifetime of the extension, multiple requests can be handled
 *  by it. For every request that is handled, an environment object is created.
 *
 *  The base class for the environment is defined in this file. If you'd like
 *  to add state variables to the environment you can override this class and
 *  add the extra features you'd like. If you override this method, you should
 *  also override Extension::createEnvironment() to return an instance of a 
 *  different class.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
 
/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Forward definitions
 */
class Extension;
class Global;

/**
 *  Class definition
 */
class Environment
{
public:
    /**
     *  Constructor
     *  @param  extension
     */
    Environment(Extension *extension) : _extension(extension) {}
    
    /**
     *  Disable copy and move operations
     */
    Environment(const Environment &environment) = delete;
    Environment(Environment &&environment) = delete;
    
    /**
     *  Destructor
     */
    virtual ~Environment() {}
    
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
    
    /**
     *  Get access to the user supplied data
     *  @return void*
     */
    virtual void *data()
    {
        return _data;
    }
    
    /**
     *  Change the user supplied data
     *  @param  data
     */
    virtual void setData(void *data)
    {
        _data = data;
    }
    
    /**
     *  Get access to a global variable
     *  @param  name
     *  @return Global
     */
    Global operator[](const char *name);
    
    /**
     *  Get access to a global variable
     *  @param  name
     *  @return Global
     */
    Global operator[](const std::string &name);
    
    

protected:
    /**
     *  The extension that this environment belongs to
     *  @var Extension*
     */
    Extension *_extension;
    
    /**
     *  Pointer to user supplied data
     *  @var void*
     */
    void *_data = NULL;
};

/**
 *  End of namespace
 */
}

