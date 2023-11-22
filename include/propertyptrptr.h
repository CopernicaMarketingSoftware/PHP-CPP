/**
 *  PropertyPtrPtr.h
 *
 *  "Interface" that can be "implemented" by your class. If you do, you
 *  create your class like this:
 *
 *  class MyClass : public Php::Base, public Php::PropertyPtrPtr { ... }
 *
 *  @author Kirill Morozov <kir.morozov@gmail.com>
 *  @copyright 2020 Morozov
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class PHPCPP_EXPORT PropertyPtrPtr
{
public:

    /**
     *  Retrieve a member
     *  @param  key
     *  @return value
     */
    virtual Php::Value getPropertyPtrPtr(const Php::Value &member, int type) = 0;

    /**
     *  Destructor
     */
    virtual ~PropertyPtrPtr() = default;
};

/**
 *  End namespace
 */
}
