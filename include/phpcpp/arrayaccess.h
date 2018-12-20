/**
 *  ArrayAccess.h
 *
 *  "Interface" that can be "implemented" by your class. If you do, you
 *  create your class like this:
 *
 *  class MyClass : public Php::Base, public Php::ArrayAccess { ... }
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT ArrayAccess
{
public:
    /**
     *  Check if a member is set
     *  @param  key
     *  @return bool
     */
    virtual bool offsetExists(const Php::Value &key) = 0;

    /**
     *  Set a member
     *  @param  key
     *  @param  value
     */
    virtual void offsetSet(const Php::Value &key, const Php::Value &value) = 0;

    /**
     *  Retrieve a member
     *  @param  key
     *  @return value
     */
    virtual Php::Value offsetGet(const Php::Value &key) = 0;

    /**
     *  Remove a member
     *  @param key
     */
    virtual void offsetUnset(const Php::Value &key) = 0;

    /**
     *  Destructor
     */
    virtual ~ArrayAccess() = default;
};

/**
 *  End namespace
 */
}
