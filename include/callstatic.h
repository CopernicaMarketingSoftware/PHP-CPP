/**
 *  CallStatic.h
 *
 *  Class that performs a lot of C++11 magic to find out if the __callStatic()
 *  method was implemented by the user, and if it was, calls it
 *
 */
 
namespace Php {

/**
 *  SFINAE test to check if the __callStatic method is defined
 * 
 *  This type trait checks if the __callStatic method is defined in class T
 * 
 *  @see http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence
 */
template <typename T>
class HasCallStatic
{
    typedef char one;
    typedef long two;

    template <typename C> static one test( decltype(&C::__callStatic) ) ;
    template <typename C> static two test(...);

public:
    static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

/**
 *  Function that only exists if the class T has a __callStatic method
 *  @param  name        Name of the function
 *  @param  params      Parameters passed to the function
 *  @return Value
 */
template<typename T>
typename std::enable_if<HasCallStatic<T>::value, Value >::type
callStatic(const char *name, Parameters &params)
{
    // call the __callStatic() function
    return T::__callStatic(name, params);
}

/**
 *  Function that only exists if the class T does not have a __callStatic method
 *  @param  name        Name of the function
 *  @param  params      Parameters passed to the function
 *  @return Value
 */
template<typename T>
typename std::enable_if<!HasCallStatic<T>::value >::type
callStatic(const char *name, Parameters &params)
{
    std::cout << "has NO call static" << std::endl;
    
    return nullptr;
}

/**
 *  End namespace
 */
}

