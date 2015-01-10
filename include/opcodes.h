/**
 *  Opcodes.h
 *
 *  Class represents a set of opcodes of a PHP script that can be executed. This
 *  is an internal file that you normally do not have to instantiate yourself.
 *  Better use the Php::Script of Php::File classes.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Forward declarations
 */
struct _zend_op_array;

/**
 *  Namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Opcodes
{
public:
    /**
     *  Constructor
     *  @param  opcodes
     */
    Opcodes(struct _zend_op_array *opcodes) : _opcodes(opcodes) 
    {
        // no other initialisation is necessary
    }
    
    /**
     *  Destructor
     */
    virtual ~Opcodes();
    
    /**
     *  Are the opcodes valid?
     *  @return bool
     */
    bool valid() const
    {
        return _opcodes != nullptr;
    }
    
    /**
     *  Execute the opcodes
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

