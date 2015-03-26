/**
 *  Parameters.h
 *
 *  Wrapper around parameters that are passed to a

 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  Forward declarations
 */
class Base;

/**
 *  Class definition
 */
class PHPCPP_EXPORT Parameters : public std::vector<Value>
{
private:
    /**
     *  The base object
     *  @var Base
     */
    Base *_object = nullptr;

protected:
    /**
     *  Protected constructor
     *
     *  The constructor is protected because extension programmers are not
     *  supposed to instantiate parameters objects themselves
     *
     *  @param  object      The 'this' object
     */
    Parameters(Base *object) : _object(object) {}

public:
    /**
     *  Destructor
     */
    virtual ~Parameters() {}

    /**
     *  The object that is being called
     *  @return Base
     */
    Base *object() const
    {
        return _object;
    }
};

/**
 *  End of namespace
 */
}
