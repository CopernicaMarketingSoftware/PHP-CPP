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
    Script(const char *name, const char *source, size_t size) noexcept;
    
    /**
     *  Alternative constructor without a size
     *  @param  name        Name of the PHP script
     *  @param  source      PHP source code to be evaluated 
     */
    Script(const char *name, const char *source) noexcept : Script(name, source, ::strlen(source)) {}

    /**
     *  Alternative constructor without a name
     *  @param  source      PHP source code to be evaluated
     *  @param  size        Length of the source code
     */
    Script(const char *source, size_t size) noexcept : Script("Unknown", source, size) {}

    /**
     *  Alternative constructor without a name and without a size
     *  @param  source      PHP source code to be evaluated
     */
    Script(const char *source) noexcept : Script("Unknown", source, ::strlen(source)) {}
    
    /**
     *  Destructor
     */
    virtual ~Script();
    
    /**
     *  Is the script a valid PHP script without syntax errors?
     *  @return bool
     */
    bool valid() const
    {
        return _opcodes != nullptr;
    }
    
    /**
     *  Execute the script
     *  The return value of the script is returned
     *  @return Value
     */
    Value execute() const;
    
private:
    /**
     *  The opcodes
     *  @var zend_op_array
     */
    struct _zend_op_array *_opcodes;

};
    
/**
 *  End of namespace
 */
}
