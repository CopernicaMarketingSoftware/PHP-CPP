/**
 *  ConstantImpl.h
 *
 *  Implementation file for the constant class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 - 2019 Copernica BV
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
        ZVAL_STRINGL(&_constant.value, value, len);
    }

    /**
     *  Constructor
     *  @param  name
     *  @param  value
     */
    ConstantImpl(const char *name, const char *value) : _name(name)
    {
        // initialize the zval
        ZVAL_STRINGL(&_constant.value, value, ::strlen(value));
    }

    /**
     *  Constructor
     *  @param  name
     *  @param  value
     */
    ConstantImpl(const char *name, const std::string &value) : _name(name)
    {
        // initialize the zval
        ZVAL_STRINGL(&_constant.value, value.c_str(), value.size());
    }

    /**
     *  Destructor
     */
    virtual ~ConstantImpl() {}

    /**
     *  Add the constant to a class
     *  @param  clss        The class to add it to
     */
    void addTo(ClassBase &clss) const
    {
        // check the zval type
        switch (Z_TYPE(_constant.value)) {

        case IS_NULL:
            // set a null constant
            clss.property(_name, nullptr, Php::Const);
            break;

        case IS_LONG:
            // set a long constant (cast is necessary because php uses longs, which
            // have a different size on different platforms)
            clss.property(_name, (int64_t)Z_LVAL(_constant.value), Php::Const);
            break;

        case IS_DOUBLE:
            // set a double constant
            clss.property(_name, Z_DVAL(_constant.value), Php::Const);
            break;

        case IS_FALSE:
            // set boolean false
            clss.property(_name, false, Php::Const);
            break;

        case IS_TRUE:
            // set boolean true
            clss.property(_name, true, Php::Const);

        case IS_STRING:
            // set a string constant
            clss.property(_name, std::string(Z_STRVAL(_constant.value), Z_STRLEN(_constant.value)), Php::Const);
            break;

        default:
            // this should not happen, the constant can only be constructed as one
            // of the above types, so it should be impossible to end up here. But
            // for completeness, we are going to make a copy of the zval, and convert
            // that to a string
            zval copy = _constant.value;

            // run the copy constructor to make sure zval is correctly copied
            zval_copy_ctor(&copy);

            // convert the copy to a string
            convert_to_string(&copy);

            // set as string constant
            clss.property(_name, std::string(Z_STRVAL(copy), Z_STRLEN(copy)), Php::Const);
            break;
        }
    }

    /**
     *  Initialize the constant
     *  @param  prefix          Namespace prefix
     *  @param  module_number   The module number
     */
    void initialize(const std::string &prefix, int module_number)
    {
        // is there a namespace name involved?
        if (!prefix.empty())
        {
            // size of the name
            auto namelen = ::strlen(_name);

            // allocate memory for the full name
            _constant.name = zend_string_alloc(prefix.size() + 1 + namelen, 1);

            // copy the entire namespace name, separator and constant name
            ::strncpy(ZSTR_VAL(_constant.name), prefix.c_str(), prefix.size());
            ::strcpy (ZSTR_VAL(_constant.name) + prefix.size(), "\\");
            ::strncpy(ZSTR_VAL(_constant.name) + prefix.size() + 1, _name, namelen + 1);
        }
        else
        {
            // no namespace, we simply copy the name
            _constant.name = zend_string_init(_name, ::strlen(_name), 1);
        }

        // before 7.3 constants could simply be set
#if PHP_VERSION_ID < 70300
        // set all the other constant properties
        _constant.flags = CONST_CS | CONST_PERSISTENT;
        _constant.module_number = module_number;
#else
        // from 7.3 onwards there is a macro for setting the constant flags and module number
        ZEND_CONSTANT_SET_FLAGS(&_constant, CONST_CS | CONST_PERSISTENT, module_number);
#endif
      
        // register the zval
        zend_register_constant(&_constant);
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

