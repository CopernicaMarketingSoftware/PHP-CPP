/**
 *  ArgInfo.h
 *
 *  Internal class that wraps the Zend information about an argument
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
class ArgInfo
{
public:
    /**
     *  Constructor if this argument should be an instance of a certain class
     *  @param  name        Name of the argument
     *  @param  classname   If a specific class is required, the class type
     *  @param  null        Are NULL values allowed in stead of an instance?
     *  @param  ref         Is this a pass-by-reference argument?
     */
    ArgInfo(const std::string &name, const std::string &classname, bool null = true, bool ref = false) :
        _name(name), _classname(name), _type(objectType), _null(null), _ref(ref) {}

    /**
     *  Constructor if this argument can be anything
     *  @param  name        Name of the argument
     *  @param  type        Type hint (arrayType or callableType)
     *  @param  ref         Is this a pass-by-reference argument?
     */
    ArgInfo(const std::string &name, Type type = nullType, bool ref = false) :
        _name(name), _type(type), _null(false), _ref(ref) {}

    /**
     *  Constructor if this argument can be anything
     *  @param  name        Name of the argument
     *  @param  ref         Is this a pass-by-reference argument?
     */
    ArgInfo(const std::string &name, bool ref = false) :
        _name(name), _type(nullType), _null(false), _ref(ref) {}
    
    /**
     *  Destructor
     */
    virtual ~ArgInfo() {}

    /**
     *  Fill a zend_arg_info structure
     *  @param  info
     */
    void fill(zend_arg_info *info)
    {
        // fill all members
        info->name = _name.c_str();
        info->name_len = _name.size();
        info->class_name = _classname.size() ? _classname.c_str() : NULL;
        info->class_name_len = _classname.size();
        info->type_hint = _type;
        info->allow_null = _null;
        info->pass_by_reference = _ref;
    }

private:
    /**
     *  The argument name
     *  @var string
     */
    std::string _name;
    
    /**
     *  The class name
     *  @var string
     */
    std::string _classname;
    
    /**
     *  Type of the argument
     *  @var Type
     */
    Type _type;
    
    /**
     *  Can this argument be set to NULL?
     *  @var bool
     */
    bool _null;
    
    /**
     *  Is this a pass-by-reference argument?
     *  @var bool
     */
    bool _ref;
};

/**
 *  End of namespace
 */
}

