/**
 *  ExtensionImpl.h
 *
 *  Implementation of the extension object for the HHVM engine
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  Class definition
 */
class ExtensionImpl : public ExtensionBase
{
private:
    /**
     *  Pointer to the extension object that is filled by the extension programmer
     *  @var    Extension
     */
    Extension *_data;
    
public:
    /**
     *  Constructor
     *  @param  data        Pointer to the extension object created by the extension programmer
     *  @param  name        Name of the extension
     *  @param  version     Version identifier of the extension
     *  @param  apiversion  API version number
     */
    ExtensionImpl(Extension *data, const char *name, const char *version, int apiversion) : ExtensionBase(data) {}
    
    /**
     *  Destructor
     */
    virtual ~ExtensionImpl() {}
    
    /**
     *  Pointer to the module that is loaded by HHVM
     *  @return void*
     */
    void *module()
    {
        return nullptr;
    }
};

/**
 *  End of namespace
 */
}

