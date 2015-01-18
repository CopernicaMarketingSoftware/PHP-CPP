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
    ConstantImpl(const char *name, const Value &value) : 
        _name(name), _value(value) {}
    
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
    void initialize(const std::string &prefix, int module_number TSRMLS_DC) const
    {
        // create constant structure
        zend_constant constant;

        // copy zval
        INIT_PZVAL_COPY(&constant.value, _value._val);

        // we have to call the copy constructor to copy the entire other zval
        zval_copy_ctor(&constant.value);

        // is there a namespace name involved?
        if (prefix.size() > 0)
        {
            // size of the name
            auto namelen = ::strlen(_name);
            
            // include prefix in the full name
            constant.name_len = prefix.size() + 1 + namelen;
            constant.name = (char *)emalloc(constant.name_len);
            
            // copy the entire namespace name, separator and constant name
            ::strncpy(constant.name, prefix.c_str(), prefix.size());
            ::strncpy(constant.name + prefix.size(), "\\", 1);
            ::strncpy(constant.name + prefix.size() + 1, _name, namelen);
        }
        else
        {
            // no namespace, we simply copy the name
            constant.name_len = ::strlen(_name);
            constant.name = zend_strndup(_name, constant.name_len);
        }

        // set all the other constant properties
        constant.flags = CONST_CS | CONST_PERSISTENT;
        constant.module_number = module_number;
        
        // register the zval
        zend_register_constant(&constant TSRMLS_CC);
    }

private:
    /**
     *  The name of the constant
     *  @var    const char *
     */
    const char *_name;
    
    /**
     *  The value of the constant
     *  @var    Value
     */
    Value _value;
};
    
/**
 *  End of namespace
 */
}

