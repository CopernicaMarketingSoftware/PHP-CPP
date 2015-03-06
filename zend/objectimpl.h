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
         *  The actual object is the first member, so that casting
         *  the MixedObject to a zend_object will also result in a valid pointer
         *  @var    zend_object
         */
        zend_object php;
        
        /**
         *  Pointer to ourselves
         *  @var    ObjectImpl
         */
        ObjectImpl *self;
        
        
    } *_mixed;
    
    /**
     *  Pointer to the C++ implementation
     *  @var    Base
     */
    Base *_object;

    /**
     *  The object handle in the Zend engine
     *  @var int
     */
    int _handle;

public:
    /**
     *  Constructor
     *
     *  This will create a new object in the Zend engine.
     *
     *  @param  entry       Zend class entry
     *  @param  base        C++ object that already exists
     *  @param  refcount    The initial refcount for the object
     *  @param  tsrm_ls     Optional threading data
     */
    ObjectImpl(zend_class_entry *entry, Base *base, int refcount TSRMLS_DC)
    {
        // allocate a mixed object (for some reason this does not have to be deallocated)
        _mixed = (MixedObject *)emalloc(sizeof(MixedObject));
        
        // copy properties to the mixed object
        _mixed->php.ce = entry;
        _mixed->self = this;
        
        // store the c++ object
        _object = base;
        
        // initialize the object
        zend_object_std_init(&_mixed->php, entry TSRMLS_CC);
        
#if PHP_VERSION_ID < 50399

        // tmp variable
        zval *tmp;
    
        // initialize the properties, php 5.3 way
        zend_hash_copy(_mixed->php.properties, &entry->default_properties, (copy_ctor_func_t) zval_property_ctor, &tmp, sizeof(zval*));

#else

        // version higher than 5.3 have an easier way to initialize
        object_properties_init(&_mixed->php, entry);

#endif    

#ifdef ZTS

        // when in thread safety mode, the destruct method and free method have
        // an extra parameter holding thread information
        using DestructType = void(*)(zend_object*,unsigned int,void***);
        using FreeType = void(*)(zend_object*,void***);
    
#else

        // not in thread mode: no special parameter for the tsrm_ls variable
        using DestructType = void(*)(zend_object*, unsigned int);
        using FreeType = void(*)(zend_object*);
    
#endif
    
        // store the two destruct methods in temporary vars
        DestructType destructMethod = &ClassImpl::destructObject;
        FreeType freeMethod = &ClassImpl::freeObject;

        // the destructor and clone handlers are set to NULL. I dont know why, but they do not
        // seem to be necessary...
        _handle = zend_objects_store_put(php(), (zend_objects_store_dtor_t)destructMethod, (zend_objects_free_object_storage_t)freeMethod, NULL TSRMLS_CC);
        
        // set the initial refcount (if it is different than one, because one is the default)
        if (refcount != 1) EG(objects_store).object_buckets[_handle].bucket.obj.refcount = refcount;
        
        // the object may remember that we are its implementation object
        base->_impl = this;
    }

    /**
     *  Destructor
     */
    virtual ~ObjectImpl()
    {
        // deallocate the cpp object
        if (_object) delete _object;
    }

    /**
     *  Destruct the object
     *  @param  tsrm_ls
     */
    void destruct(TSRMLS_D)
    {
        // pass on to the default destructor
        zend_objects_free_object_storage(php() TSRMLS_CC);
        
        // destruct the object
        delete this;
    }
    
    /**
     *  Find the object based on a zval
     *  @param  val         Zval object
     *  @param  tsrm_ls     Optional pointer to thread info
     *  @return ObjectImpl
     */
    static ObjectImpl *find(zval *val TSRMLS_DC)
    {
        // retrieve the old object, which we are going to copy
        MixedObject *object = (MixedObject *)zend_object_store_get_object(val TSRMLS_CC);
        
        // done
        return object->self;
    }

    /**
     *  Find the object based on a zend_object
     *  @param  object      Zend object pointer
     *  @return ObjectImpl
     */
    static ObjectImpl *find(const zend_object *object)
    {
        // retrieve the old object, which we are going to copy
        const MixedObject *mixed = (MixedObject *)object;
        
        // done
        return mixed->self;
    }
    
    /**
     *  Retrieve the base class of the original C++ object
     *  @return Base
     */
    Base *object()
    {
        return _object;
    }
    
    /**
     *  Pointer to the PHP object
     *  @return zend_object
     */
    zend_object *php() 
    {
        return &_mixed->php;
    }
    
    /**
     *  Retrieve the handle object
     *  @return int
     */
    int handle() const
    {
        return _handle;
    }
};

/**
 *  End of namespace
 */
}

