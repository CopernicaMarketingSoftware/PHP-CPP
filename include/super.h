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
     *  @param  index number
     */
    Super(int index) : _index(index) {}
    
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
    Value operator[](const std::string &key) const;

    /**
     *  Array access operator
     *  This can be used for accessing associative arrays
     *  @param  key
     *  @return Value
     */
    Value operator[](const char *key) const;

private:
    /**
     *  Index number
     *  @var    int
     */
    int _index;
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

