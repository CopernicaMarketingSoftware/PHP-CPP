/**
 *  File.h
 *
 *  Extended script, a PHP source file name can be passed to a Php::File object
 *  to have it evaluated.
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
class File
{
public:
    /**
     *  Constructor
     * 
     *  The constructor receives a filename as parameter. It uses the normal
     *  PHP include path resolve algorithms to find the location of the file. 
     * 
     *  @param  name        the filename
     *  @param  size        size of the filename
     */
    File(const char *name, size_t size);
    
    /**
     *  Alternative constructor with just a filename
     * 
     *  @param  name        the filename
     */
    File(const char *name) : File(name, ::strlen(name)) {}

    /**
     *  Destructor
     */
    virtual ~File();
    
    /**
     *  Include the file once
     *  @return Php::Value
     */
    Value once();
    
    /**
     *  Execute the file
     *  @return Php::Value
     */
    Value execute();
    
private:
    /**
     *  The full resolved path name
     *  @var const char *
     */
    char *_path = nullptr;
    
    /**
     *  The opcodes of this file
     *  @var Opcodes
     */
    Opcodes *_opcodes = nullptr;
    
};
    
/**
 *  End of namespace
 */
}

