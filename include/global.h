/**
 *  Global variable
 *
 *  A global variable is a value that - once updated - also updates
 *  the global scope
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Forward declarations
 */
struct _zval_struct;
struct _zend_string;

/**
 *  Namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT Global : public Value
{
public:
    /**
     *  No copy constructor
     *  @param  global
     */
    Global(const Global &global) = delete;

    /**
     *  Move constructor
     *  @param  global
     */
    Global(Global &&global) _NOEXCEPT;

    /**
     *  Destructor
     */
    virtual ~Global();

    /**
     *  Assignment operator
     *  @param  global
     *  @return Global
     */
    /*
    Global &operator=(const Global &global)
    {
        // skip self assignment
        if (&global == this) return *this;

        // call base
        Value::operator=(global);

        // copy name and exists setting
        _name = global._name;
        _exists = global._exists;

        // done
        return *this;
    }
    */

    /**
     *  Move operator
     *  @param  global
     *  @return Global
     */
    /*
    Global &operator=(Global &&global) _NOEXCEPT
    {
        // skip self assignment
        if (&global == this) return *this;

        // call base
        Value::operator=(std::move(global));

        // copy name and exists setting
        _name = std::move(global._name);
        _exists = global._exists;

        // done
        return *this;
    }
    */

    /**
     *  Assignment operator
     *  @param  value
     *  @return Global
     */
    template <typename T>
    Global &operator=(const T &value)
    {
        Value::operator=(value);

        return update();
    }

    /**
     *  Set a certain property
     *  Calling this method will turn the value into an array
     *  @param  index       Index of the property to set
     *  @param  value       Value to set
     *  @return Value       The value that was set
     */
    virtual void set(int index, const Value &value) override
    {
        // call base
        Value::set(index, value);

        // update current object
        _exists = false;
        update();
    }

    /**
     *  Set a certain property
     *  Calling this method will turn the value into an array
     *  @param  key         Key of the property to set
     *  @param  size        Size of the key
     *  @param  value       Value to set
     *  @return Value       The value that was set
     */
    virtual void set(const char *key, int size, const Value &value) override
    {
        // call base
        Value::set(key, size, value);

        // update current object
        _exists = false;
        update();
    }


protected:
    /**
     *  Function that is called when the value is updated
     *  @return Value
     */
    Global &update();

private:
    /**
     *  Constructor for non-existing var
     *
     *  @param  name    Name for the variable that does not exist
     */
    Global(const char *name);

    /**
     *  Alternative constructor for non-existing var
     *  @param  name
     */
    Global(const std::string &name);

    /**
     *  Constructor to wrap zval for existing global bar
     *  @param  name
     *  @param  val
     */
    Global(const char *name, struct _zval_struct *val);

    /**
     *  Alternative constructor to wrap zval
     *  @param  name
     *  @param  val
     */
    Global(const std::string &name, struct _zval_struct *val);

    /**
     *  Name of the variable
     *  @var struct _zend_string*
     */
    struct _zend_string *_name;

    /**
     *  Does it already exist?
     *  @var bool
     */
    bool _exists;

    /**
     *  The globals can access the private method from this class
     */
    friend class Globals;
};

/**
 *  End of namespace
 */
}
