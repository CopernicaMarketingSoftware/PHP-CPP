/**
 *  ConstantImpl.h
 * 
 *  Implementation file for the constant class
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
class ConstantImpl
{
public:
    /**
     *  Constructor
     *  @param  name
     *  @param  value
     */
    ConstantImpl(const char *name, std::nullptr_t value = nullptr) : _name(name)
    {
        // initialize the zval
        ZVAL_NULL(&_constant.value);
    }
    
    /**
     *  Constructor
     *  @param  name
     *  @param  value
     */
    ConstantImpl(const char *name, bool value) : _name(name)
    {
        // initialize the zval
        ZVAL_BOOL(&_constant.value, value);
    }
    
    /**
     *  Constructor
     *  @param  name
     *  @param  value
     */
    ConstantImpl(const char *name, int32_t value) : _name(name)
    {
        // initialize the zval
        ZVAL_LONG(&_constant.value, value);
    }

    /**
     *  Constructor
     *  @param  name
     *  @param  value
     */
    ConstantImpl(const char *name, int64_t value) : _name(name)
    {
        // initialize the zval
        ZVAL_LONG(&_constant.value, value);
    }

    /**
     *  Constructor
     *  @param  name
     *  @param  value
     */
    ConstantImpl(const char *name, double value) : _name(name)
    {
        // initialize the zval
        ZVAL_DOUBLE(&_constant.value, value);
    }
    
    /**
     *  Constructor
     *  @param  name
     *  @param  value
     *  @param  len
     */
    ConstantImpl(const char *name, const char *value, size_t len) : _name(name)
    {
        // initialize the zval
        ZVAL_STRINGL(&_constant.value, value, len, 0);
    }
    
    /**
     *  Constructor
     *  @param  name
     *  @param  value
     */
    ConstantImpl(const char *name, const char *value) : _name(name)
    {
        // initialize the zval
        ZVAL_STRINGL(&_constant.value, value, ::strlen(value), 0);
    }
    
    /**
     *  Constructor
     *  @param  name
     *  @param  value
     */
    ConstantImpl(const char *name, const std::string &value) : _name(name)
    {
        // initialize the zval
        ZVAL_STRINGL(&_constant.value, value.c_str(), value.size(), 0);
    }
    
    /**
     *  Destructor
     */
    virtual ~ConstantImpl() {}

    /**
     *  Initialize the constant
     *  @param  prefix          Namespace prefix
     *  @param  module_number   The module number
     *  @param  tsrmls          Optional parameter when running in multi-threading context
     */
    void initialize(const std::string &prefix, int module_number TSRMLS_DC) 
    {
        // is there a namespace name involved?
        if (prefix.size() > 0)
        {
            // size of the name
            auto namelen = ::strlen(_name);
            
            // include prefix in the full name (name_len should include '\0')
            _constant.name_len = prefix.size() + 1 + namelen + 1;
            _constant.name = (char *)emalloc(_constant.name_len);

            // copy the entire namespace name, separator and constant name
            ::strncpy(_constant.name, prefix.c_str(), prefix.size());
            ::strncpy(_constant.name + prefix.size(), "\\", 1);
            ::strncpy(_constant.name + prefix.size() + 1, _name, namelen + 1);
        }
        else
        {
            // no namespace, we simply copy the name (name_len should include '\0')
            _constant.name_len = ::strlen(_name) + 1;
            _constant.name = zend_strndup(_name, _constant.name_len - 1);
        }
        
        // set all the other constant properties
        _constant.flags = CONST_CS | CONST_PERSISTENT;
        _constant.module_number = module_number;
        
        // register the zval
        zend_register_constant(&_constant TSRMLS_CC);
    }

private:
    /**
     *  Name of the constant
     *  @var    const char *
     */
    const char *_name;

    /**
     *  The zend_constant structure
     *  @var    zend_constant
     */
    zend_constant _constant;
};
    
/**
 *  End of namespace
 */
}

