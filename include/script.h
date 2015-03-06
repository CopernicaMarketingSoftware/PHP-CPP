/**
 *  Script.h
 *
 *  Class that can be used to evaluate a PHP script in the current PHP context.
 *
 *  The difference between directly calling eval() is that the script object
 *  will first evaluate the string, and then it can be executed multiple times.
 *  
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Forward declarations
 */
struct _zend_op_array;

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Forward declarations
 */
class Opcodes;

/**
 *  Class definition
 */
class Script
{
public:
    /**
     *  Constructor
     * 
     *  The constructor will not throw any exceptions, even when invalid
     *  PHP code is passed to it that can not be evaluated. You should call
     *  the valid() to find out if the script was valid (could be parsed).
     * 
     *  @param  name        Name of the PHP script
     *  @param  source      PHP source code to be evaluated 
     *  @param  size        Length of the source code
     */
    Script(const char *name, const char *source, size_t size) _NOEXCEPT;
    
    /**
     *  Alternative constructor without a size
     *  @param  name        Name of the PHP script
     *  @param  source      PHP source code to be evaluated 
     */
    Script(const char *name, const char *source) _NOEXCEPT : Script(name, source, ::strlen(source)) {}

    /**
     *  Alternative constructor without a name
     *  @param  source      PHP source code to be evaluated
     *  @param  size        Length of the source code
     */
    Script(const char *source, size_t size) _NOEXCEPT : Script("Unknown", source, size) {}

    /**
     *  Alternative constructor without a name and without a size
     *  @param  source      PHP source code to be evaluated
     */
    Script(const char *source) _NOEXCEPT : Script("Unknown", source, ::strlen(source)) {}
    
    /**
     *  Constructor based on a std::string
     *  @param  source      PHP source code to be evaluated
     */
    Script(const std::string &source) _NOEXCEPT : Script("Unknown", source.c_str(), source.size()) {}
    
    /**
     *  Destructor
     */
    virtual ~Script();
    
    /**
     *  Is the script a valid PHP script without syntax errors?
     *  @return bool
     */
    bool valid() const;
    
    /**
     *  Execute the script
     *  The return value of the script is returned
     *  @return Value
     */
    Value execute() const;
    
private:
    /**
     *  The opcodes
     *  @var Opcodes
     */
    Opcodes *_opcodes;
    
    /**
     *  Helper function to compile the source code
     *  @param  name        name of the script
     *  @param  script      actual PHP code
     *  @param  size        length of the string
     *  @return opcodes
     */
    static struct _zend_op_array *compile(const char *name, const char *phpcode, size_t size);

};
    
/**
 *  End of namespace
 */
}
