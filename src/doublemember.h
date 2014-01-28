/**
 *  DoubleMember.h
 *
 *  Implementation for a property that is initially set to a boolean value
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class DoubleMember : public MemberInfo
{
private:
    /**
     *  The value
     *  @var double
     */
    double _value;
    
public:
    /**
     *  Constructor
     *  @param  value
     */
    DoubleMember(double value) : MemberInfo(), _value(value) {}

    /**
     *  Destructor
     */
    virtual ~DoubleMember() {}

    /**
     *  Is this a property member
     *  @return bool
     */
    virtual bool isProperty() { return true; }

    /**
     *  Virtual method to declare the property
     *  @param  entry       Class entry
     *  @param  name        Name of the member
     *  @param  size        Size of the name
     *  @param  flags       Additional flags
     */
    virtual void declare(struct _zend_class_entry *entry, const char *name, int size, int flags)
    {
#if PHP_VERSION_ID >= 50400
        zend_declare_property_double(entry, name, size, _value, flags);
#else
        zend_declare_property_double(entry, (char *)name, size, _value, flags);
#endif
    }


    /**
     *  Virtual method to declare the class constant
     *  @param  entry       Class entry
     *  @param  name        Name of the member
     *  @param  size        Size of the name
     *  @param  flags       Additional flags
     */
    virtual void declareConst(struct _zend_class_entry *entry, const char *name, int size)
    {
#if PHP_VERSION_ID >= 50400
        //zend_declare_class_constant_double(entry, name, size, _value);
        zend_declare_class_constant_double(entry, name, size, _value);
        std::cout << "zend_declare_class_constant_double(" << name << ", " << _value << ")" << std::endl;
#else
        zend_declare_class_constant_double(entry, (char *)name, size, _value);
#endif

/*
ZEND_API int zend_declare_class_constant         (zend_class_entry *ce, const char *name, size_t name_length, zval *value TSRMLS_DC);
ZEND_API int zend_declare_class_constant_long    (zend_class_entry *ce, const char *name, size_t name_length, long value TSRMLS_DC);
ZEND_API int zend_declare_class_constant_bool    (zend_class_entry *ce, const char *name, size_t name_length, zend_bool value TSRMLS_DC);
ZEND_API int zend_declare_class_constant_stringl (zend_class_entry *ce, const char *name, size_t name_length, const char *value, size_t value_length TSRMLS_DC);
ZEND_API int zend_declare_class_constant_string  (zend_class_entry *ce, const char *name, size_t name_length, const char *value TSRMLS_DC);

        ZEND_API int zend_declare_property       (zend_class_entry *ce, const char *name, int name_length, zval *property, int access_type TSRMLS_DC);
        ZEND_API int zend_declare_property_bool  (zend_class_entry *ce, const char *name, int name_length, long value, int access_type TSRMLS_DC);
        ZEND_API int zend_declare_property_long  (zend_class_entry *ce, const char *name, int name_length, long value, int access_type TSRMLS_DC);
        ZEND_API int zend_declare_property_string(zend_class_entry *ce, const char *name, int name_length, const char *value, int access_type TSRMLS_DC);
        ZEND_API int zend_declare_property_stringl(zend_class_entry *ce, const char *name, int name_length, const char *value, int value_len, int access_type TSRMLS_DC);
*/


    }

};

/**
 *  End of namespace
 */
}

