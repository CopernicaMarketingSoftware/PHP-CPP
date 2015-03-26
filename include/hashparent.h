/**
 *  HashParent.h
 *
 *  Interface that is implemented by all objects that can be accessed with
 *  array-access variables ([]). When the value of a hash-member is changed,
 *  it will call one of the methods from this class to set the new property
 *
 *  This is an internal class that you normally not need when writing
 *  extensions. It is used by the PHP-CPP library when you use constructs
 *  like value["x"]["y"] = 10;
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Forwards
 */
class Value;

/**
 *  Class definition
 */
class PHPCPP_EXPORT HashParent
{
protected:
    /**
     *  Protected constructor - users should not instantiate HashParent
     *  objects themselved. Use a Value object instead.
     */
    HashParent() {}

public:
    /**
     *  Destructor
     */
    virtual ~HashParent() {}

    /**
     *  Check if a certain key exists in the array/object
     *  @param  key
     *  @return bool
     */
    virtual bool contains(const std::string &key) const = 0;

    /**
     *  Check if a certain index exists in the array/object
     *  @param  key
     *  @return bool
     */
    virtual bool contains(int index) const = 0;

    /**
     *  Check if a certain index exists in the array/object
     *  @param  key
     *  @return bool
     */
    virtual bool contains(const Value &index) const = 0;

    /**
     *  Retrieve the value at a string index
     *  @param  key
     *  @return Value
     */
    virtual Value get(const std::string &key) const = 0;

    /**
     *  Retrieve the value at a numeric index
     *  @param  index
     *  @return Value
     */
    virtual Value get(int index) const = 0;

    /**
     *  Retrieve the value at a value index
     *  @param  key
     *  @return Value
     */
    virtual Value get(const Value &key) const = 0;

    /**
     *  Overwrite the value at a certain string index
     *  @param  key
     *  @param  value
     */
    virtual void set(const std::string &key, const Value &value) = 0;

    /**
     *  Overwrite the value at a certain numeric index
     *  @param  index
     *  @param  value
     */
    virtual void set(int index, const Value &value) = 0;

    /**
     *  Overwrite the value at a certain variant index
     *  @param  key
     *  @param  value
     */
    virtual void set(const Value &key, const Value &value) = 0;

    /**
     *  Unset a member by its index
     *  @param  index
     */
    virtual void unset(int index) = 0;

    /**
     *  Unset a member by its key
     *  @param  key
     */
    virtual void unset(const std::string &key) = 0;

    /**
     *  Unset a member by its key
     *  @param  key
     */
    virtual void unset(const Value &key) = 0;

};

/**
 *  End namespace
 */
}
