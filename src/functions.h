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
 *  Set up namespace
 */
namespace PhpCpp {

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
    Functions(const std::initializer_list<Function> &functions) : _functions(functions)
    {
        // allocate the function entries
        _entries = new zend_function_entry[functions.size() + 1];
        
        // keep iterator counter
        int i = 0;
        
        // loop through the functions
        for (auto it = begin(functions); it != functions.end(); it++)
        {
            // let the callable fill the array
            it->internal()->fill(&_entries[i++]);
        }
        
        // last entry should be set to all zeros
        zend_function_entry *last = &_entries[i];
        
        // all should be set to zero
        memset(last, 0, sizeof(zend_function_entry));
    }

    /**
     *  Destructor
     */
    virtual ~Functions()
    {
        delete[] _entries;
    }
    
    /**
     *  Retrieve the internal data
     *  @return zend_function_entry*
     */
    zend_function_entry *internal()
    {
        return _entries;
    }


private:
    /**
     *  The internal entries
     *  @var zend_function_entry*
     */
    zend_function_entry *_entries;
    
    /**
     *  Vector of functions (we need this because the function objects must
     *  remain in memory)
     *  @var vector
     */
    std::vector<Function> _functions;
};

/**
 *  End of namespace
 */
}

