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
class ArrayAccess
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
     *  Alternative offsetExists as it is initially called
     *  @param  params
     *  @return bool
     */
    virtual Php::Value offsetExists(Php::Parameters &params)
    {
        return offsetExists(params[0]);
    }
    
    /**
     *  Alternative set member function as it is initially called
     *  @param  params
     */
    virtual void offsetSet(const Php::Parameters &params)
    {
        offsetSet(params[0], params[1]);
    }
    
    /**
     *  Alternative retrieve member function that is initially called
     *  @param  params
     *  @return value
     */
    virtual Php::Value offsetGet(Php::Parameters &params)
    {
        return offsetGet(params[0]);
    }
    
    /**
     *  Alternative function to remove a member that is initally called
     *  @param params
     */
    virtual void offsetUnset(Php::Parameters &params)
    {
        return offsetUnset(params[0]);
    }
};
    
/**
 *  End namespace
 */
}

