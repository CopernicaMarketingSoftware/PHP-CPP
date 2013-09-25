/**
 *  Class.h
 *
 *  When a class is registered in the extension, you need this helper class
 *  for that.
 *
 *  The use of it is simple:
 *
 *  Extension::add(Class<YourClass>);
 *
 *  Note that YourClass must extend from Php::Object
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */
 
/**
 *  Set up namespace
 */
namespace Php {
 
/**
 *  Class definition of the class
 */
template <typename T>
class Class
{
public:
    /**
     *  Constructor
     */
    Class() {}
    
    /**
     *  Move constructor
     *  @param  that
     */
    Class(Class &&that) {}

    /**
     *  Copy constructor
     */
    Class(const Class &that) {}
    
    /**
     *  Destructor
     */
    virtual ~Class() {}
    
    /**
     *  Construct an instance
     *  @return Base
     */
    Base* construct()
    {
        // allocate the object
        return new T();
    }

protected:

};

/**
 *  End of namespace
 */
}

