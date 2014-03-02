/**
 *  Base class for defining your own objects
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Base
{
protected:
    /**
     *  Constructor
     */
    Base() {}

public:
    /**
     *  Virtual destructor
     */
    virtual ~Base() {}

    /**
     *  Convert the object to a Php::Value object (how it is used externally)
     *  @return Value
     */
    Value value() const;
    
    /**
     *  Get access to a property by name
     *  @param  string
     *  @return Value
     */
    Value operator[](const char *name) const
    {
        return value()[name];
    }

    /**
     *  Alternative way to access a property
     *  @param  string
     *  @return Value
     */
    Value operator[](const std::string &name) const
    {
        return value()[name];
    }
    
private:
    /**
     *  The zend_object
     *  @var    zend_object
     */
    struct _zend_object *_object = nullptr;
    
    /**
     *  Private method to assign the zend object
     *  @param  zend_object
     */
    void assign(struct _zend_object *object)
    {
        // copy pointer
        _object = object;
    }
    
    /**
     *  ClassBase has access to private data
     */
    friend class ClassBase;
};


/**
 *  End of namespace
 */
}

