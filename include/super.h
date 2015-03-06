/**
 *  Super.h
 *
 *  The Super class is used to implement one of the super variables $_POST,
 *  $_GET, $_SERVER, et cetera
 *
 *  @copyright 2014 Copernica BV
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 */

/**
 *  Set up namespace
 */
namespace Php {
    
/**
 *  Class definition
 */
class Super
{
public:
    /**
     *  Constructor
     * 
     *  Extension writers do not have to access the super-globals themselves.
     *  They are always accessible via Php::POST, Php::GET, et cetera.
     * 
     *  @param  index   index number
     *  @param  name    name of the variable in PHP
     */
    Super(int index, const char *name) : _index(index), _name(name) {}
    
    /**
     *  Destructor
     */
    virtual ~Super() {}
    
    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return Value
     */
    Value operator[](const std::string &key)
    {
        // convert object to a value object, and retrieve the key
        return value()[key];
    }

    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return Value
     */
    Value operator[](const char *key)
    {
        // convert object to a value object, and retrieve the key
        return value()[key];
    }

    /**
     *  Casting operator to cast to a value object
     *  @return Value
     */
    operator Value ()
    {
        // we have a private function for this
        return value();
    }

    /**
     *  Return an iterator for iterating over the variables
     *  @return iterator
     */
    ValueIterator begin()
    {
        // convert to value, and call begin on the value object
        return value().begin();
    }
    
    /**
     *  Return an iterator for iterating over the variables
     *  @return iterator
     */
    ValueIterator end()
    {
        // convert to value, and call end on that object
        return value().end();
    }

private:
    /**
     *  Index number
     *  @var    int
     */
    int _index;
    
    /**
     *  Name of the variable in PHP
     *  @var    name
     */
    const char *_name;
    
    /**
     *  Turn the object into a value object
     *  @return Value
     */
    Value value();
    
};

/**
 *  A number of super-globals are always accessible
 */
extern Super POST;
extern Super GET;
extern Super COOKIE;
extern Super SERVER;
extern Super ENV;
extern Super FILES;
extern Super REQUEST;

/**
 *  End namespace
 */
}

