/**
 *  Super.h
 *
 *  The Super class is used to implement one of the super variables $_POST,
 *  $_GET, $_SERVER, et cetera
 *
 *  @copyright 2014 Copernica BV
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 */

#include <cstring>

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT Super
{
public:
    /**
     *  Constructor
     *
     *  Extension writers do not have to access the super-globals themselves.
     *  They are always accessible via Php::POST, Php::GET, et cetera.
     *
     *  @param  name    name of the variable in PHP
     */
    Super(const char *name) : _ziag_called(false), _name(name) {}

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
        return getKey(key.c_str(), key.size());
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
        return getKey(key, std::strlen(key));
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
     *  Whether @c zend_is_auto_global() was called for this superglobal
     */
    bool _ziag_called;

    /**
     *  Name of the variable in PHP
     *  @var    name
     */
    const char *_name;

    /**
     * Calls @c zend_is_auto_global() if necessary and looks up this superglobal in the symbol table
     * @internal
     */
    struct _zval_struct* resolve();

    /**
     *  Turn the object into a value object
     *  @return Value
     */
    Value value();

    /**
     * Returns @c SUPERGLOBAL[key].
     * If there is no @c key index, it gets created
     */
    Value getKey(const char* key, std::size_t len);
};

/**
 *  A number of super-globals are always accessible
 */
extern PHPCPP_EXPORT Super POST;
extern PHPCPP_EXPORT Super GET;
extern PHPCPP_EXPORT Super COOKIE;
extern PHPCPP_EXPORT Super SERVER;
extern PHPCPP_EXPORT Super ENV;
extern PHPCPP_EXPORT Super FILES;
extern PHPCPP_EXPORT Super REQUEST;

/**
 *  End namespace
 */
}
