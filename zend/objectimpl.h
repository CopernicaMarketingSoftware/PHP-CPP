/**
 *  ObjectImpl.h
 *
 *  Implementation class for Base objects that allow the objects to be stored
 *  in the Zend engine
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
class ObjectImpl
{
private:
    /**
     *  Structure with a first element which is a zend_object, so that
     *  it can be casted to a zend_object
     *  @var    MixedObject
     */
    struct MixedObject
    {
        /**
         *  Pointer to ourselves
         *  @var    ObjectImpl
         */
        ObjectImpl *self;

        /**
         *  The actual object MUST be the last member, because PHP uses hackish
         *  tricks for optimization (we allocate more memory than sizeof(MixedObject))
         *  @var    zend_object
         */
        zend_object php;

    } *_mixed;

    /**
     *  Pointer to the C++ implementation
     *  @var    std::unique_ptr<Base>
     */
    std::unique_ptr<Base> _object;

public:
    /**
     *  Constructor
     *
     *  This will create a new object in the Zend engine.
     *
     *  @param  entry       Zend class entry
     *  @param  handler     Zend object handlers
     *  @param  base        C++ object that already exists
     *  @param  refcount    The initial refcount for the object
     */
    ObjectImpl(zend_class_entry *entry, Base *base, zend_object_handlers *handlers, int refcount) :
        _object(base)
    {
        // allocate a mixed object (for some reason this does not have to be deallocated)
        _mixed = (MixedObject *)ecalloc(1, sizeof(MixedObject) + zend_object_properties_size(entry));

        // copy properties to the mixed object
        _mixed->php.ce = entry;
        _mixed->self = this;

        // initialize the object and its properties
        zend_object_std_init  (&_mixed->php, entry);
        object_properties_init(&_mixed->php, entry);

        // install the handlers
        _mixed->php.handlers = handlers;
#if PHP_VERSION_ID < 70300
        // set the initial refcount (if it is different than one, because one is the default)
        if (refcount != 1) GC_REFCOUNT(php()) = refcount;
#else
        // set the initial refcount (if it is different than one, because one is the default)
        if (refcount != 1) GC_SET_REFCOUNT(php(),refcount);
#endif
        // the object may remember that we are its implementation object
        base->_impl = this;
    }

    /**
     *  Destructor
     */
    virtual ~ObjectImpl()
    {
        zend_object_std_dtor(&_mixed->php);
    }

    /**
     *  Destruct the object
     */
    void destruct()
    {
        // destruct the object
        delete this;
    }

    /**
     *  The offset between the zend_object and the ObjectImpl
     *  in bytes. This can be used to find the other when only
     *  a pointer to one is available.
     *
     *  @return The offset in bytes
     */
    static constexpr size_t offset()
    {
        // calculate the offset in bytes
        return offsetof(MixedObject, php);
    }

    /**
     *  Find the object based on a zval
     *  @param  val         Zval object
     *  @return ObjectImpl
     */
    static ObjectImpl *find(zval *val)
    {
        // retrieve the zend_object from the zval and use it to find the ObjectImpl
        return find(Z_OBJ_P(val));
    }

    /**
     *  Find the object based on a zend_object
     *  @param  object      Zend object pointer
     *  @return ObjectImpl
     */
    static ObjectImpl *find(const zend_object *object)
    {
        // the zend_object is the last pointer in the struct so we have to subtract the
        // correct number of bytes from the pointer to get at the address at which the
        // actual ObjectImpl starts. to be able to actually perform this pointer arithmetic
        // we must first cast the pointer to a char (void pointer arithmetic is not allowed!)
        auto *mixed = (const MixedObject*)((char*)object - offset());

        // done
        return mixed->self;
    }

    /**
     *  Retrieve the base class of the original C++ object
     *  @return Base
     */
    Base *object() const
    {
        return _object.get();
    }

    /**
     *  Pointer to the PHP object
     *  @return zend_object
     */
    zend_object *php() const
    {
        return &_mixed->php;
    }
};

/**
 *  End of namespace
 */
}

