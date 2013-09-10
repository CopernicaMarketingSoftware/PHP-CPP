/**
 *  Function.h
 *
 *  Object represents a callable function that is defined with the CPP API.
 *  After you've instantiated the extension, you can add function objects to
 *  it.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Forward definitions
 */
struct _zend_function_entry;
struct _zend_internal_function_info;
 
/**
 *  Set up namespace
 */
namespace Php {

/** 
 *  Class definition
 */
class Function
{
public:
    /**
     *  Constructor
     * 	@param	name	Name of the function
     *  @param  min     Min number of arguments
     *  @param  max     Max number of arguments
     */
    Function(const char *name, int min = 0, int max = 0) : _ptr(this, name)
    {
        // construct the arguments
        _arguments = std::shared_ptr<Arguments>(new Arguments(min, max));
    }
    
    /**
     *  No copy constructor
     *  @param  function    The other function
     */
    Function(const Function &function) : _ptr(this, function.name())
    {
        // copy members
        _arguments = function._arguments;
        _type = function._type;
    }

    /**
     *  Move constructor
     *  @param  function    The other function
     */
    Function(Function &&function) : _ptr(this, function.name())
    {
        // copy arguments
        _arguments = function._arguments;
        _type = function._type;

        // no longer need the other arguments
        function._arguments.reset();
    }
    
    /**
     *  Destructor
     */
    virtual ~Function() {}
    
    /**
     *  Assignment operator
     *  @param  function    The other function
     *  @return Function
     */
    Function &operator=(const Function &function)
    {
        // skip self reference
        if (this == &function) return *this;
        
        // copy members
        _ptr.setText(function.name());
        _arguments = function._arguments;
        _type = function._type;
        
        // done
        return *this;
    }
    
    /**
     *  Comparison
     *  @param	function	The other function
     * 	@return	bool
     */
    bool operator<(const Function &function) const
    {
		return strcmp(name(), function.name()) < 0;
	}

    /**
     *  Comparison
     *  @param	function	The other function
     * 	@return	bool
     */
    bool operator>(const Function &function) const
    {
		return strcmp(name(), function.name()) > 0;
	}

    /**
     *  Comparison
     *  @param	function	The other function
     * 	@return	bool
     */
    bool operator==(const Function &function) const
    {
		return strcmp(name(), function.name()) == 0;
	}

	/**
	 *  Function name
	 *  @return	const char *
	 */
	const char *name() const
	{
		return _ptr.text();
	}

    /**
     *  Method that gets called every time the function is executed
     *  @param  request     Request object
     *  @param  params      The parameters that were passed
     *  @return Variable    Return value
     */
    virtual Value invoke(Request &request, Parameters &params)
    {
        return nullptr;
    }

protected:
    /**
     *  Suggestion for the return type
     *  @var    Type
     */
    Type _type = nullType;

    /**
     *  Pointer to the arguments
     *  @var    shared_ptr
     */
    std::shared_ptr<Arguments> _arguments;

    /**
     *  The name is stored in a hidden pointer, so that we have access to the function
     *  @var	HiddenPointer
     */
    HiddenPointer<Function> _ptr;

private:
    /**
     *  Fill a function entry
     *  @param  entry       Entry to be filled
     */
    void fill(struct _zend_function_entry *entry) const;

    /**
     *  Fill function info
     *  @param  info        Info object to be filled
     */
    void fill(struct _zend_internal_function_info *info) const;
    
    /**
     *  Extension has access to the private members
     */
    friend class Extension;

};

/**
 *  End of namespace
 */
}

