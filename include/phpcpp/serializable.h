/**
 *  Serializable interface
 *
 *  This interface can be implemented to make an object that can be passed to
 *  the PHP serialize() and unserialize() methods.
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
class PHPCPP_EXPORT Serializable
{
public:
    /**
     *  Method to serialize the object
     *
     *  This method should return a string representation of the object that
     *  can be passed to the unserialize() method and that will revive the object
     *
     *  @return std::string
     */
    virtual std::string serialize() = 0;

    /**
     *  Unserialize the object
     *
     *  This method is called as an alternative __construct() method to initialize
     *  the object. The passed in string parameter in in the format earlier returned
     *  by a call to serialize()
     *
     *  @param  input           String to parse
     *  @param  size            Size of the string
     */
    virtual void unserialize(const char *input, size_t size) = 0;
};

/**
 *  End namespace
 */
}
