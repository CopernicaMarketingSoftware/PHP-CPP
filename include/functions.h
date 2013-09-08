/**
 *  Functions.h
 *
 *  Internal helper class that parses the functions initializer list, and
 *  that converts it into a zend_function_entry array.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Define structures
 */
struct _zend_function_entry;

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Functions
{
public:
    /**
     *  Constructor
     *  @param  functions   The functions to parse
     */
    Functions(const std::initializer_list<Function> &functions);

    /**
     *  Destructor
     */
    virtual ~Functions();

private:    
    /**
     *  Retrieve the internal data
     *  @return zend_function_entry*
     */
    _zend_function_entry *internal() const
    {
        return _entries;
    }

    /**
     *  The internal entries
     *  @var zend_function_entry*
     */
    _zend_function_entry *_entries;
    
    /**
     *  Vector of functions (we need this because the function objects must
     *  remain in memory, so that we can call the invoke methods on them)
     *  @var vector
     */
    std::vector<Function> _functions;
    
    /**
     *  The extension has access to the private elements
     */
    friend class Extension;
};

/**
 *  End of namespace
 */
}

