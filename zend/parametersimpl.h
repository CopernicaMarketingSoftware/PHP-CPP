/**
 *  ParametersImpl.h
 *
 *  Extended parameters class that can be instantiated
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class ParametersImpl : public Parameters
{
public:
    /**
     *  Constructor
     *  @param  this_ptr    Pointer to the object
     *  @param  argc        Number of arguments
     */
    ParametersImpl(zval *this_ptr, uint32_t argc) : Parameters(this_ptr ? ObjectImpl::find(this_ptr)->object() : nullptr)
    {
        // reserve plenty of space
        reserve(argc);

        // array to store all the arguments in
        zval arguments[argc];

        // retrieve the arguments
        zend_get_parameters_array_ex(argc, arguments);

        // loop through the arguments
        for (uint32_t i=0; i<argc; i++)
        {
            // append value
            emplace_back(&arguments[i]);
        }
    }

    /**
     *  Do _not_ add a virtual destructor here.
     *
     *  We are extending a vector, which does not itself
     *  have a virtual destructor, so destructing through
     *  a pointer to this vector has no effect.
     *
     *  By adding a virtual destructor we create a vtable,
     *  which makes the class bigger, causing slicing and
     *  then we are actually introducing the problem that
     *  we are trying to avoid!
     */
};

/**
 *  End of namespace
 */
}

