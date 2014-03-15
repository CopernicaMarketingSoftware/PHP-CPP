/**
 *  ClassBase.cpp
 *
 *  Implementation of the ClassBase class.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Retrieve our C++ implementation object
 *  @param  entry
 *  @return ClassBase
 */
static ClassBase *cpp_class(zend_class_entry *entry)
{
    // we need the base class (in user space the class may have been overridden,
    // but we are not interested in these user space classes)
    while (entry->parent) entry = entry->parent;
    
#if PHP_VERSION_ID >= 50400
    // retrieve the comment (it has a pointer hidden in it to the ClassBase object)
    const char *comment = entry->info.user.doc_comment;
#else
    // retrieve the comment php5.3 style (it has a pointer hidden in it to the ClassBase object)
    const char *comment = entry->doc_comment;
#endif    
    
    // the first byte of the comment is an empty string (null character), but
    // the next bytes contain a pointer to the ClassBase class
    return *((ClassBase **)(comment + 1));
}

/**
 *  Retrieve the CPP object
 *  @param  val
 *  @return Base
 */
static Base *cpp_object(const zval *val)
{
    // retrieve the old object, which we are going to copy
    MixedObject *object = (MixedObject *)zend_object_store_get_object(val);

    // return the cpp object
    return object->cpp;
}

/**
 *  Protected constructor
 *  @param  classname   Class name
 *  @param  flags       Class flags
 */
ClassBase::ClassBase(const char *classname, int flags) : _name(classname)
{
    // the flags hold a method-flag-value, this should be converted into a class-type
    if (flags & Abstract) _type = ClassType::Abstract;
    if (flags & Final) _type = ClassType::Final;
}

/**
 *  Destructor
 */
ClassBase::~ClassBase()
{
    // destruct the entries
    if (_entries) delete[] _entries;

    // php 5.3 deallocates the doc_comment by iself
#if PHP_VERSION_ID >= 50400    
    if (_comment) free(_comment);
#endif
}

/**
 *  Extended zend_internal_function structure that we use to store an
 *  instance of the ClassBase object. We need this for static method calls
 */
struct CallData
{
    // the internal function is the first member, so
    // that it is possible to cast an instance of this 
    // struct to a zend_internal_function
    zend_internal_function func;
    
    // and a pointer to the ClassBase object
    ClassBase *self;
};

/**
 *  Handler function that runs the __call function
 *  @param  ...     All normal parameters for function calls
 */
void ClassBase::callMethod(INTERNAL_FUNCTION_PARAMETERS)
{
    // retrieve the originally called (and by us allocated) function object
    // (this was copied from the zend engine source code, code looks way too
    // ugly to be made by me)
    CallData *data = (CallData *)EG(current_execute_data)->function_state.function;
    zend_internal_function *func = &data->func;

    // retrieve the function name
    const char *name = func->function_name;
    ClassBase *meta = data->self;
    
    // the data structure was allocated by ourselves in the getMethod or 
    // getStaticMethod functions, we no longer need it now
    efree(data);

    // the function could throw an exception
    try
    {
        // wrap the return value
        Value result(return_value, true);

        // construct parameters
        Parameters params(this_ptr, ZEND_NUM_ARGS());

        // retrieve the base object
        Base *base = params.object();
        
        // is this a static, or a non-static call?
        if (base) result = meta->callCall(base, name, params);
        else result = meta->callCallStatic(name, params);
    }
    catch (const NotImplemented &exception)
    {
        // because of the two-step nature, we are going to report the error ourselves
        zend_error(E_ERROR, "Undefined method %s", name);
    }
    catch (Exception &exception)
    {
        // process the exception
        exception.process();
    }
}

/**
 *  Handler function that runs the __invoke function
 *  @param  ...     All normal parameters for function calls
 */
void ClassBase::callInvoke(INTERNAL_FUNCTION_PARAMETERS)
{
    // retrieve the originally called (and by us allocated) function object
    // (this was copied from the zend engine source code, code looks way too
    // ugly to be made by me)
    CallData *data = (CallData *)EG(current_execute_data)->function_state.function;

    // get self reference
    ClassBase *meta = data->self;

    // the data structure was allocated by ourselves in the getMethod or 
    // getStaticMethod functions, we no longer need it now
    efree(data);

    // the function could throw an exception
    try
    {
        // wrap the return value
        Value result(return_value, true);

        // construct parameters
        Parameters params(this_ptr, ZEND_NUM_ARGS());

        // retrieve the base object
        Base *base = params.object();

        // call the actual __invoke method on the base object
        result = meta->callInvoke(base, params);
    }
    catch (const NotImplemented &exception)
    {
        // because of the two-step nature, we are going to report the error ourselves
        zend_error(E_ERROR, "Function name must be a string");

    }
    catch (Exception &exception)
    {
        // process the exception
        exception.process();
    }
}

/**
 *  Method that returns the function definition of the __call function
 *  @param  object_ptr
 *  @param  method_name
 *  @param  method_len
 *  @return zend_function
 */
#if PHP_VERSION_ID < 50399
zend_function *ClassBase::getMethod(zval **object_ptr, char *method_name, int method_len)
#else
zend_function *ClassBase::getMethod(zval **object_ptr, char *method_name, int method_len, const struct _zend_literal *key)
#endif
{
    // something strange about the Zend engine (once more). The structure with
    // object-handlers has a get_method and call_method member. When a function is
    // called, the get_method function is called first, to retrieve information
    // about the method (like the handler that should be called to execute it),
    // after that, this returned handler is also called. The call_method property
    // of the object_handlers structure however, never gets called. Typical.
    
    // first we'll check if the default handler does not have an implementation,
    // in that case the method is probably already implemented as a regular method
#if PHP_VERSION_ID < 50399
    auto *defaultFunction = std_object_handlers.get_method(object_ptr, method_name, method_len);
#else
    auto *defaultFunction = std_object_handlers.get_method(object_ptr, method_name, method_len, key);
#endif

    // did the default implementation do anything?
    if (defaultFunction) return defaultFunction;
    
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(*object_ptr);

    // this is peculiar behavior of the zend engine, we first are going to dynamically 
    // allocate memory holding all the properties of the __call method (we initially
    // had an implementation here that used a static variable, and that worked too,
    // but we'll follow thread safe implementation of the Zend engine here, although
    // it is strange to allocate and free memory in one and the same method call (free()
    // call happens in call_method())
    auto *data = (CallData *)emalloc(sizeof(CallData));
    auto *function = &data->func;
    
    // we're going to set all properties
    function->type = ZEND_INTERNAL_FUNCTION;
    function->module = nullptr;
    function->handler = &ClassBase::callMethod;
    function->arg_info = nullptr;
    function->num_args = 0;
    function->required_num_args = 0;
    function->scope = entry;
    function->fn_flags = ZEND_ACC_CALL_VIA_HANDLER;
    function->function_name = method_name;
    
    // store pointer to ourselves
    data->self = cpp_class(entry);
    
    // done (cast to zend_function* is allowed, because a zend_function is a union
    // that has one member being a zend_internal_function)
    return (zend_function *)data;
}

/**
 *  Method that is called right before a static method call is attempted
 *  @param  entry
 *  @param  method
 *  @param  method_len
 *  @return zend_function
 */
zend_function *ClassBase::getStaticMethod(zend_class_entry *entry, char* method, int method_len)
{
    // first we'll check if the default handler does not have an implementation,
    // in that case the method is probably already implemented as a regular method
#if PHP_VERSION_ID < 50399
    auto *defaultFunction = zend_std_get_static_method(entry, method, method_len);
#else
    auto *defaultFunction = zend_std_get_static_method(entry, method, method_len, nullptr);
#endif

    // did the default implementation do anything?
    if (defaultFunction) return defaultFunction;

    // just like we did in getMethod() (see comment there) we are going to dynamically
    // allocate data holding information about the function
    auto *data = (CallData *)emalloc(sizeof(CallData));
    auto *function = &data->func;
    
    // we're going to set all properties
    function->type = ZEND_INTERNAL_FUNCTION;
    function->module = nullptr;
    function->handler = ClassBase::callMethod;
    function->arg_info = nullptr;
    function->num_args = 0;
    function->required_num_args = 0;
    function->scope = nullptr;
    function->fn_flags = ZEND_ACC_CALL_VIA_HANDLER;
    function->function_name = method;
    
    // store pointer to ourselves
    data->self = cpp_class(entry);
    
    // done (cast to zend_function* is allowed, because a zend_function is a union
    // that has one member being a zend_internal_function)
    return (zend_function *)data;
}

/**
 *  Method that returns the closure -- this is the __invoke handler!
 *  @param  object
 *  @param  entry_ptr
 *  @param  func
 *  @param  object_ptr
 *  @return int
 */
int ClassBase::getClosure(zval *object, zend_class_entry **entry_ptr, zend_function **func, zval **object_ptr)
{
    // it is really unbelievable how the Zend engine manages to implement every feature
    // in a complete different manner. You would expect the __invoke() and the
    // __call() functions not to be very different from each other. However, they
    // both have a completely different API. This getClosure method is supposed
    // to fill the function parameter with all information about the invoke()
    // method that is going to get called
    
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(object);

    // just like we did for getMethod(), we're going to dynamically allocate memory
    // with all information about the function
    auto *data = (CallData *)emalloc(sizeof(CallData));
    auto *function = &data->func;
    
    // we're going to set all properties
    function->type = ZEND_INTERNAL_FUNCTION;
    function->module = nullptr;
    function->handler = &ClassBase::callInvoke;
    function->arg_info = nullptr;
    function->num_args = 0;
    function->required_num_args = 0;
    function->scope = entry;
    function->fn_flags = ZEND_ACC_CALL_VIA_HANDLER;
    function->function_name = nullptr;
    
    // store pointer to ourselves
    data->self = cpp_class(entry);
    
    // assign this dynamically allocated variable to the func parameter
    // the case is ok, because zend_internal_function is a member of the
    // zend_function union
    *func = (zend_function *)data;
 
    // the object_ptr should be filled with the object on which the method is 
    // called (otherwise the Zend engine tries to call the method statically)
    *object_ptr = object;
    
    // done
    return SUCCESS;
};

/**
 *  Retrieve pointer to our own object handlers
 *  @return zend_object_handlers
 */
zend_object_handlers *ClassBase::objectHandlers()
{
    // keep static structure
    static zend_object_handlers handlers;
    
    // is the object already initialized?
    static bool initialized = false;
    
    // already initialized?
    if (initialized) return &handlers;
    
    // initialize the handlers
    memcpy(&handlers, &std_object_handlers, sizeof(zend_object_handlers));
    
    // install custom clone function
    handlers.clone_obj = clonable() ? &ClassBase::cloneObject : nullptr;
    
    // functions for the Countable interface
    handlers.count_elements = &ClassBase::countElements;
    
    // functions for the ArrayAccess interface
    handlers.write_dimension = &ClassBase::writeDimension;
    handlers.read_dimension = &ClassBase::readDimension;
    handlers.has_dimension = &ClassBase::hasDimension;
    handlers.unset_dimension = &ClassBase::unsetDimension;
    
    // functions for the magic properties handlers (__get, __set, __isset and __unset)
    handlers.write_property = &ClassBase::writeProperty;
    handlers.read_property = &ClassBase::readProperty;
    handlers.has_property = &ClassBase::hasProperty;
    handlers.unset_property = &ClassBase::unsetProperty;
    
    // when a method is called (__call and __invoke)
    handlers.get_method = &ClassBase::getMethod;
    handlers.get_closure = &ClassBase::getClosure;
    
    // handler to cast to a different type
    handlers.cast_object = &ClassBase::cast;
    
    // method to compare two objects
    handlers.compare_objects = &ClassBase::compare;
    
    // remember that object is now initialized
    initialized = true;
    
    // done
    return &handlers;
}

/**
 *  Function to compare two objects
 *  @param  object1
 *  @param  object2
 *  @return int
 */
int ClassBase::compare(zval *object1, zval *object2)
{
    // prevent exceptions
    try
    {
        // retrieve the class entry linked to this object
        auto *entry = zend_get_class_entry(object1);

        // other object must be of the same type
        if (entry != zend_get_class_entry(object2)) throw NotImplemented();

        // we need the C++ class meta-information object
        ClassBase *meta = cpp_class(entry);
        
        // get the base objects
        Base *base1 = cpp_object(object1);
        Base *base2 = cpp_object(object2);
        
        // run the compare method
        return meta->callCompare(base1, base2);
    }
    catch (const NotImplemented &exception)
    {
        // it was not implemented, do we have a default?
        if (!std_object_handlers.compare_objects) return 1;
        
        // call default
        return std_object_handlers.compare_objects(object1, object2);
    }
    catch (Exception &exception)
    {
        // a Php::Exception was thrown by the extension __compare function, 
        // pass this on to user space
        exception.process();
        
        // what shall we return here...
        return 1;
    }
}

/**
 *  Function to cast the object to a different type
 *  @param  object
 *  @param  retval
 *  @param  type
 *  @return int
 */
int ClassBase::cast(zval *object, zval *retval, int type)
{
    // get the base object
    Base *base = cpp_object(object);
    
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(object);

    // we need the C++ class meta-information object
    ClassBase *meta = cpp_class(entry);
    
    // retval it not yet initialized --- and again feelings of disbelief,
    // frustration, wonder and anger come up when you see that there are not two
    // functions in the Zend engine that have a comparable API
    INIT_PZVAL(retval);
    
    // wrap zval in value object
    Value result(retval, true);
    
    // when the magic function it not implemented, an exception will be thrown,
    // and the extension may throw a Php::Exception
    try
    {
        // the result zval
        zval *result = nullptr;
        
        // check type
        switch ((Type)type) {
        case Type::Numeric:     result = meta->callToInteger(base).detach();  break;
        case Type::Float:       result = meta->callToFloat(base).detach();    break;
        case Type::Bool:        result = meta->callToBool(base).detach();     break;
        case Type::String:      result = meta->callToString(base).detach();   break;
        default:                throw NotImplemented();             break;
        }
        
        // @todo do we turn into endless conversion if the __toString object returns 'this' ??
        // (and if it does: who cares? If the extension programmer is stupid, why do we have to suffer?)
        
        // is the object overwritten?
        if (object == retval) zval_dtor(object);
        
        // overwrite the result
        ZVAL_ZVAL(retval, result, 1, 1);
    
        // done
        return SUCCESS;
    }
    catch (const NotImplemented &exception)
    {
        // is there a default?
        if (!std_object_handlers.cast_object) return FAILURE;
        
        // call default
        return std_object_handlers.cast_object(object, retval, type);
    }
    catch (Exception &exception)
    {
        // pass on the exception to php userspace
        exception.process();
        
        // done
        return FAILURE;
    }
}
    
/**
 *  Function that is called to create space for a cloned object
 *  @param  val                     The object to be cloned
 *  @return zend_obejct_value       The object to be created
 */
zend_object_value ClassBase::cloneObject(zval *val TSRMLS_DC)
{
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(val);

    // we need the C++ class meta-information object
    ClassBase *meta = cpp_class(entry);

    // retrieve the old object, which we are going to copy
    MixedObject *old_object = (MixedObject *)zend_object_store_get_object(val);

    // create a new base c++ object
    auto *cpp = meta->clone(old_object->cpp);
    
    // report error on failure (this does not occur because the cloneObject()
    // method is only installed as handler when we have seen that there is indeed
    // a copy constructor)
    if (!cpp) throw Php::Exception(std::string("Unable to clone ") + entry->name);

    // the thing we're going to return
    zend_object_value result;
    
    // set the handlers
    result.handlers = meta->objectHandlers();
    
    // store the object
    MixedObject *new_object = cpp->store(entry);

    // store the object in the object cache
    result.handle = cpp->handle();
    
    // clone the members (this will also call the __clone() function if the user
    // had registered that as a visible method)
    zend_objects_clone_members(&new_object->php, result, &old_object->php, Z_OBJ_HANDLE_P(val));
    
    // was a custom clone method installed? If not we call the magic c++ __clone method
    if (!entry->clone) meta->callClone(cpp);
    
    // done
    return result;
}

/**
 *  Function that is used to count the number of elements in the object
 * 
 *  If the user has implemented the Countable interface, this method will 
 *  call the count() method
 * 
 *  @param  val
 *  @param  count
 *  @return int
 */
int ClassBase::countElements(zval *object, long *count TSRMLS_DC)
{
    // does it implement the countable interface?
    Countable *countable = dynamic_cast<Countable*>(cpp_object(object));

    // if it does not implement the Countable interface, we rely on the default implementation
    if (countable) 
    {
        // the user function may throw an exception that needs to be processed
        try
        {
            // call the count function
            *count = countable->count();
            
            // done
            return SUCCESS;
        }
        catch (Exception &exception)
        {
            // process the exception
            exception.process();
            
            // unreachable
            return FAILURE;
        }
    }
    else
    {
        // Countable interface was not implemented, check if there is a default
        if (!std_object_handlers.count_elements) return FAILURE;
        
        // call default
        return std_object_handlers.count_elements(object, count);
    }
}

/**
 *  Function that is called when the object is used as an array in PHP
 * 
 *  This is the [] operator in PHP, and mapped to the offsetGet() method 
 *  of the ArrayAccess interface
 * 
 *  @param  object          The object on which it is called
 *  @param  offset          The name of the property
 *  @param  type            The type of the variable???
 *  @return zval
 */
zval *ClassBase::readDimension(zval *object, zval *offset, int type)
{
    // what to do with the type?
    //
    // the type parameter tells us whether the dimension was read in READ
    // mode, WRITE mode, READWRITE mode or UNSET mode. 
    // 
    // In 99 out of 100 situations, it is called in regular READ mode (value 0), 
    // only when it is called from a PHP script that has statements like 
    // $x =& $object["x"], $object["x"]["y"] = "something" or unset($object["x"]["y"]), 
    // the type parameter is set to a different value.
    //
    // But we must ask ourselves the question what we should be doing with such
    // cases. Internally, the object most likely has a full native implementation,
    // and the property that is returned is just a string or integer or some
    // other value, that is temporary WRAPPED into a zval to make it accessible
    // from PHP. If someone wants to get a reference to such an internal variable,
    // that is in most cases simply impossible.
    
     
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(cpp_object(object));
    
    // if it does not implement the ArrayAccess interface, we rely on the default implementation
    if (arrayaccess) 
    {
        // the C++ code may throw an exception
        try
        {
            // ArrayAccess is implemented, call function
            return toZval(arrayaccess->offsetGet(offset), type);
        }
        catch (Exception &exception)
        {
            // process the exception (send it to user space)
            exception.process();
            
            // unreachable
            return Value(nullptr).detach();
        }
    }
    else
    {
        // ArrayAccess not implemented, check if there is a default handler
        if (!std_object_handlers.read_dimension) return nullptr;
        
        // call default
        return std_object_handlers.read_dimension(object, offset, type);
    }
}

/**
 *  Function that is called when the object is used as an array in PHP
 * 
 *  This is the [] operator in PHP, and mapped to the offsetSet() method 
 *  of the ArrayAccess interface
 * 
 *  @param  object          The object on which it is called
 *  @param  offset          The name of the property
 *  @param  value           The new value
 *  @return zval
 */
void ClassBase::writeDimension(zval *object, zval *offset, zval *value)
{
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(cpp_object(object));
    
    // if it does not implement the ArrayAccess interface, we rely on the default implementation
    if (arrayaccess) 
    {
        // method may throw an exception
        try
        {
            // set the value
            arrayaccess->offsetSet(offset, value);
        }
        catch (Exception &exception)
        {
            // process the exception (send it to user space
            exception.process();
        }
    }
    else
    {
        // ArrayAccess not interface, check if there is a default handler
        if (!std_object_handlers.write_dimension) return;
        
        // call the default
        std_object_handlers.write_dimension(object, offset, value);
    }
}

/**
 *  Function that is called when the object is used as an array in PHP
 * 
 *  This is the [] operator in PHP, and mapped to the offsetExists() method
 *  of the ArrayAccess interface
 * 
 *  @param  object          The object on which it is called
 *  @param  member          The member to check
 *  @param  check_empty     Was this an isset() call, or an empty() call?
 *  @return bool
 */
int ClassBase::hasDimension(zval *object, zval *member, int check_empty)
{
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(cpp_object(object));
    
    // if it does not implement the ArrayAccess interface, we rely on the default implementation
    if (arrayaccess) 
    {
        // user implemented callbacks could throw an exception
        try
        {
            // check if the member exists
            if (!arrayaccess->offsetExists(member)) return false;
            
            // we know for certain that the offset exists, but should we check
            // more, like whether the value is empty or not?
            if (!check_empty) return true;

            // the user wants to know if the property is empty
            return empty(arrayaccess->offsetGet(member));
        }
        catch (Exception &exception)
        {
            // process the exception (send it to user space)
            exception.process();
            
            // unreachable
            return false;
        }
    }
    else
    {
        // ArrayAccess interface is not implemented, check if there is a default handler
        if (!std_object_handlers.has_dimension) return 0;

        // call default
        return std_object_handlers.has_dimension(object, member, check_empty);
    }
}

/**
 *  Function that is called when the object is used as an array in PHP
 * 
 *  This is the [] operator in PHP, and mapped to the offsetUnset() method
 *  of the ArrayAccess interface
 * 
 *  @param  object          The object on which it is called
 *  @param  member          The member to remove
 */
void ClassBase::unsetDimension(zval *object, zval *member)
{
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(cpp_object(object));
    
    // if it does not implement the ArrayAccess interface, we rely on the default implementation
    if (arrayaccess) 
    {
        // user implemented code could throw an exception
        try
        {
            // remove the member
            arrayaccess->offsetUnset(member);
        }
        catch (Exception &exception)
        {
            // process the exception (send it to user space)
            exception.process();
        }
    }
    else
    {
        // ArrayAccess is not implemented, is a default handler available?
        if (!std_object_handlers.unset_dimension) return;
        
        // call the default
        std_object_handlers.unset_dimension(object, member);
    }
}

/**
 *  Helper method to turn a property into a zval
 *  @param  value
 *  @param  type
 *  @return Value
 */
zval *ClassBase::toZval(Value &&value, int type)
{
    // because we do not want the value object to destruct the zval when
    // it falls out of scope, we detach the zval from it, if this is a regular
    // read operation we can do this right away
    if (type == 0) return value.detach();

    // this is a more complicated read operation, the scripts wants to get
    // deeper access to the returned value. This, however, is only possible
    // if the value has more than once reference (if it has a refcount of one,
    // the value object that we have here is the only instance of the zval,
    // and it is simply impossible to return a reference or so
    if (value.refcount() <= 1) return value.detach(); 

    // we're dealing with an editable zval, return a reference variable
    return Value(value.detach(), true).detach();
}

/**
 *  Function that is called when a property is read
 *  @param  object
 *  @param  name
 *  @param  type
 *  @param  key
 *  @return val
 */
#if PHP_VERSION_ID < 50399
zval *ClassBase::readProperty(zval *object, zval *name, int type)
#else
zval *ClassBase::readProperty(zval *object, zval *name, int type, const struct _zend_literal *key)
#endif
{
    // what to do with the type?
    //
    // the type parameter tells us whether the property was read in READ
    // mode, WRITE mode, READWRITE mode or UNSET mode. 
    // 
    // In 99 out of 100 situations, it is called in regular READ mode (value 0), 
    // only when it is called from a PHP script that has statements like 
    // $x =& $object->x, $object->x->y = "something" or unset($object->x->y)
    // the type parameter is set to a different value.
    //
    // But we must ask ourselves the question what we should be doing with such
    // cases. Internally, the object most likely has a full native implementation,
    // and the property that is returned is just a string or integer or some
    // other value, that is temporary WRAPPED into a zval to make it accessible
    // from PHP. If someone wants to get a reference to such an internal variable,
    // that is in most cases simply impossible.

    // retrieve the object and class
    Base *base = cpp_object(object);
    
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(object);

    // we need the C++ class meta-information object
    ClassBase *meta = cpp_class(entry);

    // the default implementation throws an exception, so by catching 
    // the exception we know if the object was implemented by the user or not
    try
    {
        // convert name to a Value object
        Value key(name);
        
        // is it a property with a callback?
        auto iter = meta->_properties.find(key);
        
        // was it found?
        if (iter == meta->_properties.end())
        {
            // retrieve value from the __get method
            return toZval(meta->callGet(base, key), type);
        }
        else
        {
            // get the value
            return toZval(iter->second->get(base), type);
        }
    }
    catch (const NotImplemented &exception)
    {
        // __get() function was not overridden by the user
        if (!std_object_handlers.read_property) return nullptr;
        
        // call default
#if PHP_VERSION_ID < 50399
        return std_object_handlers.read_property(object, name, type);
#else
        return std_object_handlers.read_property(object, name, type, key);
#endif
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method 
        // implementation, send it to user space
        exception.process();
        
        // unreachable
        return Value(nullptr).detach();
    }
}

/**
 *  Function that is called when a property is set / updated
 * 
 *  This is the handler for the __set() function, and is called when a property
 *  is updated.
 * 
 *  @param  object          The object on which it is called
 *  @param  name            The name of the property
 *  @param  value           The new value
 *  @param  key             ???
 *  @return zval
 */
#if PHP_VERSION_ID < 50399
void ClassBase::writeProperty(zval *object, zval *name, zval *value)
#else
void ClassBase::writeProperty(zval *object, zval *name, zval *value, const struct _zend_literal *key)
#endif
{
    // retrieve the object and class
    Base *base = cpp_object(object);
    
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(object);

    // we need the C++ class meta-information object
    ClassBase *meta = cpp_class(entry);

    // the default implementation throws an exception, if we catch that
    // we know for sure that the user has not overridden the __set method
    try
    {
        // wrap the name
        Value key(name);
        
        // check if the property has a callback
        auto iter = meta->_properties.find(key);
        
        // is it set?
        if (iter == meta->_properties.end())
        {
            // use the __set method
            meta->callSet(base, key, value);
        }
        else
        {
            // check if it could be set
            if (iter->second->set(base, value)) return;
            
            // read-only property
            zend_error(E_ERROR, "Unable to write to read-only property %s", (const char *)key);
        }
    }
    catch (const NotImplemented &exception)
    {
        // __set() function was not overridden by user, check if there is a default
        if (!std_object_handlers.write_property) return;
        
        // call the default
#if PHP_VERSION_ID < 50399
        std_object_handlers.write_property(object, name, value);
#else
        std_object_handlers.write_property(object, name, value, key);
#endif
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method 
        // implementation, send it to user space
        exception.process();
    }
}

/**
 *  Function that is called to check whether a certain property is set
 *  for an object
 * 
 *  This is the handler for the __isset() function, and is called when a PHP
 *  script checks if a certain property is set.
 * 
 *  The has_set_exists parameter can have the following values:
 *
 *      0 (has) whether property exists and is not NULL
 *      1 (set) whether property exists and is true
 *      2 (exists) whether property exists
 * 
 *  @param  object          The object on which it is called
 *  @param  name            The name of the property to check
 *  @param  has_set_exists  See above
 *  @param  key             ???
 *  @return bool
 */
#if PHP_VERSION_ID < 50399
int ClassBase::hasProperty(zval *object, zval *name, int has_set_exists)
#else
int ClassBase::hasProperty(zval *object, zval *name, int has_set_exists, const struct _zend_literal *key)
#endif
{
    // the default implementation throws an exception, if we catch that
    // we know for sure that the user has not overridden the __isset method
    try
    {
        // get the cpp object
        Base *base = cpp_object(object);
        
        // retrieve the class entry linked to this object
        auto *entry = zend_get_class_entry(object);

        // we need the C++ class meta-information object
        ClassBase *meta = cpp_class(entry);
        
        // convert the name to a Value object
        Value key(name);

        // check if this is a callback property
        if (meta->_properties.find(key) != meta->_properties.end()) return true;

        // call the C++ object
        if (!meta->callIsset(base, key)) return false;
        
        // property exists, but what does the user want to know
        if (has_set_exists == 2) return true;
        
        // we have to retrieve the property
        Value value = meta->callGet(base, key);
        
        // should we check on NULL?
        switch (has_set_exists) {
        case 0:     return value.type() != Type::Null;
        default:    return value.boolValue();
        }
    }
    catch (const NotImplemented &exception)
    {
        // __isset was not implemented, do we have a default?
        if (!std_object_handlers.has_property) return 0;

        // call default
#if PHP_VERSION_ID < 50399
        return std_object_handlers.has_property(object, name, has_set_exists);
#else
        return std_object_handlers.has_property(object, name, has_set_exists, key);
#endif
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method 
        // implementation, send it to user space
        exception.process();
        
        // unreachable
        return false;
    }
}

/**
 *  Function that is called when a property is removed from the project
 * 
 *  This is the handler for the __unset() method
 * 
 *  @param  object          The object on which it is called
 *  @param  member          The member to remove
 *  @param  key
 */
#if PHP_VERSION_ID < 50399
void ClassBase::unsetProperty(zval *object, zval *member)
#else
void ClassBase::unsetProperty(zval *object, zval *member, const struct _zend_literal *key)
#endif
{
    // the default implementation throws an exception, if we catch that
    // we know for sure that the user has not overridden the __unset method
    try
    {
        // retrieve the class entry linked to this object
        auto *entry = zend_get_class_entry(object);

        // we need the C++ class meta-information object
        ClassBase *meta = cpp_class(entry);
        
        // property name
        Value name(member);
        
        // is this a callback property?
        auto iter = meta->_properties.find(name);
        
        // if the property does not exist, we forward to the __unset
        if (iter == meta->_properties.end()) meta->callUnset(cpp_object(object), member);
        
        // callback properties cannot be unset
        zend_error(E_ERROR, "Property %s can not be unset", (const char *)name);
    }
    catch (const NotImplemented &exception)
    {
        // __unset was not implemented, do we have a default?
        if (!std_object_handlers.unset_property) return;
        
        // call the default
#if PHP_VERSION_ID < 50399
        std_object_handlers.unset_property(object, member);
#else
        std_object_handlers.unset_property(object, member, key);
#endif
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method 
        // implementation, send it to user space
        exception.process();
    }
}

/**
 *  Function that is called when an object is about to be destructed
 *  This will call the magic __destruct method
 */
void ClassBase::destructObject(zend_object *object, zend_object_handle handle)
{
    // allocate memory for the object
    MixedObject *obj = (MixedObject *)object;
    
    // get meta info
    ClassBase *meta = cpp_class(object->ce);
    
    // prevent exceptions
    try
    {
        // call the destruct function
        if (obj->cpp) meta->callDestruct(obj->cpp);
    }
    catch (const NotImplemented &exception)
    {
        // fallback on the default destructor call
        zend_objects_destroy_object(object, handle);
    }
    catch (Exception &exception)
    {
        // a regular Php::Exception was thrown by the extension, pass it on
        // to PHP user space
        exception.process();
    }
}

/**
 *  Function that is called to clean up space that is occupied by the object
 *  @param  object      The object to be deallocated
 */
void ClassBase::freeObject(zend_object *object)
{
    // allocate memory for the object
    MixedObject *obj = (MixedObject *)object;
    
    // deallocate the cpp object
    if (obj->cpp) delete obj->cpp;
    
    // pass on to the default destructor
    zend_objects_free_object_storage(object);
}

/**
 *  Function that is called when an instance of the class needs to be created.
 *  This function will create the C++ class, and the PHP object
 *  @param  entry                   Pointer to the class information
 *  @return zend_object_value       The newly created object
 */
zend_object_value ClassBase::createObject(zend_class_entry *entry TSRMLS_DC)
{
    // we need the C++ class meta-information object
    ClassBase *meta = cpp_class(entry);

    // create a new base C++ object
    auto *cpp = meta->construct();

    // report error on failure
    if (!cpp) throw Php::Exception(std::string("Unable to instantiate ") + entry->name);

    // the thing we're going to return
    zend_object_value result;
    
    // set the handlers
    result.handlers = meta->objectHandlers();
    
    // store the object
    cpp->store(entry);

    // store the object in the object cache
    result.handle = cpp->handle();
    
    // done
    return result;
}

/**
 *  Function to create a new iterator to iterate over an object
 *  @param  entry                   The class entry
 *  @param  object                  The object to iterate over
 *  @param  by_ref                  ?????
 *  @return zend_object_iterator*   Pointer to the iterator
 */
zend_object_iterator *ClassBase::getIterator(zend_class_entry *entry, zval *object, int by_ref)
{
    // by-ref is not possible (copied from SPL)
    if (by_ref) throw Php::Exception("Foreach by ref is not possible");
    
    // retrieve the traversable object
    Traversable *traversable = dynamic_cast<Traversable*>(cpp_object(object));
    
    // user may throw an exception in the getIterator() function
    try
    {
        // create an iterator
        auto *iterator = traversable->getIterator();
        
        // return the implementation
        return iterator->implementation();
    }
    catch (Exception &exception)
    {
        // user threw an exception in its method 
        // implementation, send it to user space
        exception.process();
        
        // unreachable
        return nullptr;
    }
}

/**
 *  Method that is called to serialize an object
 *  @param  object      The object to be serialized
 *  @param  buffer      Buffer in which to store the data
 *  @param  buf_len     Size of the bufffer
 *  @param  data        ??
 *  @return int
 */
int ClassBase::serialize(zval *object, unsigned char **buffer, zend_uint *buf_len, zend_serialize_data *data)
{
    // get the serializable object
    Serializable *serializable = dynamic_cast<Serializable*>(cpp_object(object));
    
    // call the serialize method on the object
    auto value = serializable->serialize();

    // allocate the buffer, and copy the data into it (the zend engine will 
    // (hopefully) clean up the data for us - the default serialize method does
    // it like this too)
    *buffer = (unsigned char*)estrndup(value.c_str(), value.size());
    *buf_len = value.size();

    // done
    return SUCCESS;
}

/**
 *  Method that is called to unserialize an object
 *  @param  object      The object to be unserialized
 *  @param  entry       The class entry to which is belongs
 *  @param  buffer      Buffer holding the unserialized data
 *  @param  data        All the unserialize data
 *  @return int
 */
int ClassBase::unserialize(zval **object, zend_class_entry *entry, const unsigned char *buffer, zend_uint buf_len, zend_unserialize_data *data)
{
    // create the PHP object
    object_init_ex(*object, entry);
    
    // turn this into a serializale
    Serializable *serializable = dynamic_cast<Serializable*>(cpp_object(*object));
    
    // call the unserialize method on it
    serializable->unserialize((const char *)buffer, buf_len);
    
    // done
    return SUCCESS;
}

/**
 *  Retrieve an array of zend_function_entry objects that hold the 
 *  properties for each method. This method is called at extension
 *  startup time to register all methods.
 * 
 *  @param  classname       The class name
 *  @return zend_function_entry[]
 */
const struct _zend_function_entry *ClassBase::entries()
{
    // already initialized?
    if (_entries) return _entries;
    
    // allocate memory for the functions
    _entries = new zend_function_entry[_methods.size() + 1];
    
    // keep iterator counter
    int i = 0;

    // loop through the functions
    for (auto &method : _methods)
    {
        // retrieve entry
        zend_function_entry *entry = &_entries[i++];

        // let the function fill the entry
        method->initialize(entry, _name);
    }

    // last entry should be set to all zeros
    zend_function_entry *last = &_entries[i];

    // all should be set to zero
    memset(last, 0, sizeof(zend_function_entry));

    // done
    return _entries;
}

/**
 *  Initialize the class, given its name
 * 
 *  The module functions are registered on module startup, but classes are
 *  initialized afterwards. The Zend engine is a strange thing. Nevertheless,
 *  this means that this method is called after the module is already available.
 *  This function will inform the Zend engine about the existence of the
 *  class.
 * 
 *  @param  prefix      namespace prefix
 */
void ClassBase::initialize(const std::string &prefix)
{
    // the class entry
    zend_class_entry entry;

    // update the name
    if (prefix.size() > 0) _name = prefix + "\\" + _name;

    // initialize the class entry
    INIT_CLASS_ENTRY_EX(entry, _name.c_str(), _name.size(), entries());

    // we need a special constructor
    entry.create_object = &ClassBase::createObject;
    
    // register function that is called for static method calls
    entry.get_static_method = &ClassBase::getStaticMethod;
    
    // for traversable classes we install a special method to get the iterator
    if (traversable()) entry.get_iterator = &ClassBase::getIterator;
    
    // for serializable classes, we install callbacks for serializing and unserializing
    if (serializable())
    {
        // add handlers to serialize and unserialize
        entry.serialize = &ClassBase::serialize;
        entry.unserialize = &ClassBase::unserialize;
    }
    
    // register the class
    _entry = zend_register_internal_class(&entry TSRMLS_CC);
    
    // allocate doc comment to contain an empty string + a hidden pointer
    if (!_comment)
    {
        // allocate now
        _comment = (char *)malloc(1 + sizeof(ClassBase *));
        
        // empty string on first position
        _comment[0] = '\0';
        
        // this pointer has to be copied to temporary pointer, as &this causes compiler error
        ClassBase *base = this;
        
        // copy the 'this' pointer to the doc-comment
        memcpy(_comment+1, &base, sizeof(ClassBase *));
    }
    
    // store pointer to the class in the unused doc_comment member
#if PHP_VERSION_ID >= 50400    
    _entry->info.user.doc_comment = _comment;
#else
    // and store the wrapper inside the comment
    _entry->doc_comment = _comment;
#endif

    // set access types flags for class
    _entry->ce_flags = (int)_type;
    
    // declare all member variables
    for (auto &member : _members) member->initialize(_entry);
}

/**
 *  Function that can be called by a derived method when a certain function
 *  is not implemented
 */
void ClassBase::notImplemented()
{
    // throw an exception
    throw NotImplemented();
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const method_callback_0 &callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const method_callback_1 &callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const method_callback_2 &callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const method_callback_3 &callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const method_callback_4 &callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const method_callback_5 &callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const method_callback_6 &callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const method_callback_7 &callback, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, callback, flags, args));
}

/**
 *  Add a static method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const native_callback_0 &method, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, method, flags | ZEND_ACC_STATIC, args));
}

/**
 *  Add a static method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const native_callback_1 &method, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, method, flags | ZEND_ACC_STATIC, args));
}

/**
 *  Add a static method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const native_callback_2 &method, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, method, flags | ZEND_ACC_STATIC, args));
}

/**
 *  Add a static method to the class
 *  @param  name        Name of the method
 *  @param  method      The actual method
 *  @param  flags       Optional flags
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, const native_callback_3 &method, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, method, flags | ZEND_ACC_STATIC, args));
}

/**
 *  Add an abstract method to the class
 *  @param  name        Name of the method
 *  @param  flags       Optional flags (like public or protected)
 *  @param  args        Description of the supported arguments
 */
void ClassBase::method(const char *name, int flags, const Arguments &args)
{
    // add the method
    _methods.push_back(std::make_shared<Method>(name, Abstract | flags, args));
}



/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, std::nullptr_t value, int flags)
{
    // add property
    _members.push_back(std::make_shared<NullMember>(name, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, int16_t value, int flags)
{
    // add property
    _members.push_back(std::make_shared<NumericMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, int32_t value, int flags)
{
    // add property
    _members.push_back(std::make_shared<NumericMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, int64_t value, int flags)
{
    // add property
    _members.push_back(std::make_shared<NumericMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, bool value, int flags)
{
    // add property
    _members.push_back(std::make_shared<BoolMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, char value, int flags)
{
    // add property
    _members.push_back(std::make_shared<StringMember>(name, &value, 1, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, const std::string &value, int flags)
{
    // add property
    _members.push_back(std::make_shared<StringMember>(name, value, flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, const char *value, int flags)
{
    // add property
    _members.push_back(std::make_shared<StringMember>(name, value, strlen(value), flags));
}

/**
 *  Add a property to the class
 *  @param  name        Name of the property
 *  @param  value       Actual property value
 *  @param  flags       Optional flags
 */
void ClassBase::property(const char *name, double value, int flags)
{
    // add property
    _members.push_back(std::make_shared<FloatMember>(name, value, flags));
}

/**
 *  Set property with callbacks
 *  @param  name        Name of the property
 *  @param  getter      Getter method
 */
void ClassBase::property(const char *name, const getter_callback_0 &getter)
{
    // add property
    _properties[name] = std::make_shared<Property>(getter);
}

/**
 *  Set property with callbacks
 *  @param  name        Name of the property
 *  @param  getter      Getter method
 */
void ClassBase::property(const char *name, const getter_callback_1 &getter)
{
    // add property
    _properties[name] = std::make_shared<Property>(getter);
}

/**
 *  Set property with callbacks
 *  @param  name        Name of the property
 *  @param  getter      Getter method
 *  @param  setter      Setter method
 */
void ClassBase::property(const char *name, const getter_callback_0 &getter, const setter_callback_0 &setter)
{
    // add property
    _properties[name] = std::make_shared<Property>(getter,setter);
}

/**
 *  Set property with callbacks
 *  @param  name        Name of the property
 *  @param  getter      Getter method
 *  @param  setter      Setter method
 */
void ClassBase::property(const char *name, const getter_callback_1 &getter, const setter_callback_0 &setter)
{
    // add property
    _properties[name] = std::make_shared<Property>(getter,setter);
}

/**
 *  Set property with callbacks
 *  @param  name        Name of the property
 *  @param  getter      Getter method
 *  @param  setter      Setter method
 */
void ClassBase::property(const char *name, const getter_callback_0 &getter, const setter_callback_1 &setter)
{
    // add property
    _properties[name] = std::make_shared<Property>(getter,setter);
}

/**
 *  Set property with callbacks
 *  @param  name        Name of the property
 *  @param  getter      Getter method
 *  @param  setter      Setter method
 */
void ClassBase::property(const char *name, const getter_callback_1 &getter, const setter_callback_1 &setter)
{
    // add property
    _properties[name] = std::make_shared<Property>(getter,setter);
}


/**
 *  End namespace
 */
}

