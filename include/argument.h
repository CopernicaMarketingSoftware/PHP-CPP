/**
 *  Argument.h
 *
 *  Class holds information about an argument that is passed to a function.
 *  You'll need this class when you're defining your own functions.
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
class ArgInfo;

/**
 *  Class definition
 */
class Argument
{
public:
    /**
     *  Constructor if this argument should be an instance of a certain class
     *  @param  name        Name of the argument
     *  @param  classname   If a specific class is required, the class type
     *  @param  null        Are NULL values allowed in stead of an instance?
     *  @param  ref         Is this a pass-by-reference argument?
     */
    Argument(const std::string &name, const std::string &classname, bool null = true, bool ref = false);

    /**
     *  Constructor if the argument can be anything
     *  Note that only arrayType and callableType are supported type-hints
     *  @param  name        Name of the argument
     *  @param  type        Type hint (arrayType or callableType)
     *  @param  ref         Is this a pass-by-reference argument?
     */
    Argument(const std::string &name, Type type = nullType, bool ref = false);

    /**
     *  Constructor if the argument can be anything
     *  @param  name        Name of the argument
     *  @param  ref         Is this a pass-by-reference argument?
     */
    Argument(const std::string &name, bool ref = false);
    
    /**
     *  Copy constructor
     *  @param  argument    The argument to copy
     */
    Argument(const Argument &argument)
    {
        // copy members
        _refcount = argument._refcount;
        _info = argument._info;
        
        // increase references
        (*_refcount)++;
    }
    
    /**
     *  Destructor
     */
    virtual ~Argument()
    {
        // cleanup current object
        cleanup();
    }

    /**
     *  Copy operator
     *  @param  argument    The argument to copy
     *  @return Argument
     */
    Argument &operator=(const Argument &argument)
    {
        // skip self assignment
        if (this == &argument) return *this;
        
        // clean up current object
        cleanup();
        
        // copy members
        _refcount = argument._refcount;
        _info = argument._info;
        
        // increase references
        (*_refcount)++;
        
        // done
        return *this;
    }
    
    /**
     *  Retrieve argument info
     *  @return ArgInfo
     *  @internal
     */
    ArgInfo *internal() const
    {
        return _info;
    }

private:
    /**
     *  Number of references
     *  @var int
     */
    int *_refcount;
    
    /**
     *  Pointer to the implementation
     *  @var ArgInfo
     */
    ArgInfo *_info;
    
    /**
     *  Remove one reference from the object
     */
    void cleanup();
    
};
    
/**
 *  End of namespace
 */
}

