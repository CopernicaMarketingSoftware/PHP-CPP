/**
 *  @file constant.h
 *
 *  If you want to define global PHP constants, or class constants you can
 *  use this constant class for it. Wrap the constant you'd like to create
 *  in a Php::Constant object and add it to the extension or the class.
 *
 *  Examples:
 *      extension.add(Php::Constant("CONSTANT_NAME", "value"));
 *      myclass.add(Php::Constant("CLASS_CONSTANT", "value"));
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Forward declarations
 */
class ConstantImpl;

/**
 *  Class definition
 */
class PHPCPP_EXPORT Constant
{
public:
    /**
     *  Constructor to create a constant for a null value
     *
     *  @param  name  Constant's name
     *  @param  value Constant's value
     */
    Constant(const char *name, std::nullptr_t value = nullptr);

    /**
     * Constructor to create a constant for a boolean value
     *
     * @param name  Constant's name
     * @param value Constant's value
     */
    Constant(const char *name, bool value);

    /**
     * Constructor to create a constant for a 32-bit integer
     *
     * @param name  Constant's name
     * @param value Constant's value
     */
    Constant(const char *name, int32_t value);

    /**
     * Constructor to create a constant for a 64-bit integer
     *
     * @param name  Constant's name
     * @param value Constant's value
     */
    Constant(const char *name, int64_t value);

    /**
     * Constructor to create a constant for a double precision
     * floating point number
     *
     * @param name  Constant's name
     * @param value Constant's value
     */
    Constant(const char *name, double value);

    /**
     * Constructor to create a constant for a string literal
     *
     * @param name  Constant's name
     * @param value Constant's value
     */
    Constant(const char *name, const char *value);

    /**
     * Constructor to create a constant for a string literal
     * specifying the length of the string
     *
     * @param name  Constant's name
     * @param value Constant's value
     * @param size  Length of the string value
     */
    Constant(const char *name, const char *value, size_t size);

    /**
     * Constructor to create a constant for a string literal
     * using a std::string
     *
     * @param name  Constant's name
     * @param value Constant's value
     */
    Constant(const char *name, const std::string &value);

    /**
     *  Destructor
     */
    virtual ~Constant() = default;

    /**
     *  Add the constant to a class
     *
     *  You normally do not have to call this method yourself. You can simply
     *  do one of the following method calls to create class constants:
     *
     *      myclass.property("MY_CONSTANT", "value", Php::Const);
     *      myclass.constant("MY_CONSTANT", "value");
     *      myclass.add(Php::Constant("MY_CONSTANT", "value"));
     *
     *  All of the calls have the same result, it is up to you to decide which
     *  one suits you best. If you use the last one - using a Php::Constant
     *  class - the PHP-CPP library will call this "addTo()" method internally
     *  to forward the call to one of the other methods.
     *
     *  @param clss Class to which the constant is added
     *  @internal
     */
    void addTo(ClassBase &clss) const;

private:
    /**
     *  Pointer to the actual implementation of the constant
     *
     *  @var std::shared_ptr Pointer to the constant implementation containing
     *                       distributed ownership properties
     */
    std::shared_ptr<ConstantImpl> _impl;

    /**
     *  Get access to the implementation object
     *
     *  @return std::shared_ptr Pointer to the constant implementation
     */
    const std::shared_ptr<ConstantImpl> &implementation() const
    { return _impl; }

    /**
     *  The extension object has access to privates
     */
    friend class ExtensionImpl;
};

/**
 *  End of namespace
 */
}
