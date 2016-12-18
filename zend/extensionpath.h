/**
 *  ExtensionPath.h
 *
 *  Simply utility class that turns a pathname for an extension into a full
 *  pathname that also takes the global extension_dir setting into account.
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
class ExtensionPath
{
private:
    /**
     *  The actual value
     *  @var std::string
     */
    std::string _path;

public:
    /**
     *  Constructor
     *  @param  path
     */
    ExtensionPath(const char *path)
    {
        // was an absole path given?
        if (path[0] && path[0] == '/')
        {
            // for absolute pathnames no extension_dir has to be checked
            _path = path;
        }
        else
        {
            // start with the extension dir
            _path = PG(extension_dir);
            
            // append slash
            if (_path[_path.size()-1] != '/') _path.push_back('/');
            
            // and append path passed to the constructor
            _path.append(path);
        }
    }
    
    /**
     *  Destructor
     */
    virtual ~ExtensionPath() {}
    
    /**
     *  Cast to string
     *  @return std::string
     */
    operator const std::string & () const
    {
        return _path;
    }
    
    /**
     *  Cast to const-char*
     *  @return const char *
     */
    operator const char * () const
    {
        return _path.c_str();
    }
};

/**
 *  End of nmaespace
 */
}

