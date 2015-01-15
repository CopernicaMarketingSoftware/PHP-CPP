/**
 *  ClassImpl.cpp
 *
 *  Implementation file for the ClassImpl class
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
 *  Destructor
 */
ClassImpl::~ClassImpl()
{
    // destruct the entries
    if (_entries) delete[] _entries;
}

/**
 *  @todo refactor so that methods become simpler
 */

/**
 *  Retrieve our C++ implementation object
 *  @param  entry
 *  @return ClassImpl
 */
static ClassImpl *self(zend_class_entry *entry)
{
    // we need the base class (in user space the class may have been overridden,
    // but we are not interested in these user space classes)
    while (entry->parent) entry = entry->parent;

#if PHP_VERSION_ID >= 50400
    // retrieve the comment (it has a pointer hidden in it to the ClassBase object)
    const char *comment = entry->info.user.doc_comment;

    // the first byte of the comment is an empty string (null character), but
    // the next bytes contain a pointer to the ClassBase class
    return *((ClassImpl **)(comment + 1));
#else
    // on php 5.3 we store the pointer to impl after the name in the entry
    ClassImpl** impl = (ClassImpl**)(entry->name + 1 + entry->name_length);

    // return the actual implementation
    return *impl;
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
    
    // and a pointer to the ClassImpl object
    ClassImpl *self;
};

/**
 *  Handler function that runs the __call function
 *  @param  ...     All normal parameters for function calls
 */
void ClassImpl::callMethod(INTERNAL_FUNCTION_PARAMETERS)
{
    // retrieve the originally called (and by us allocated) function object
    // (this was copied from the zend engine source code, code looks way too
    // ugly to be made by me)
    CallData *data = (CallData *)EG(current_execute_data)->function_state.function;
    zend_internal_function *func = &data->func;

    // retrieve the function name
    const char *name = func->function_name;
    ClassBase *meta = data->self->_base;
    
    // the data structure was allocated by ourselves in the getMethod or 
    // getStaticMethod functions, we no longer need it now
    efree(data);

    // the function could throw an exception
    try
    {
        // wrap the return value
        Value result(return_value, true);

        // construct parameters
        ParametersImpl params(this_ptr, ZEND_NUM_ARGS() TSRMLS_CC);

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
        process(exception TSRMLS_CC);
    }
}

/**
 *  Handler function that runs the __invoke function
 *  @param  ...     All normal parameters for function calls
 */
void ClassImpl::callInvoke(INTERNAL_FUNCTION_PARAMETERS)
{
    // retrieve the originally called (and by us allocated) function object
    // (this was copied from the zend engine source code, code looks way too
    // ugly to be made by me)
    CallData *data = (CallData *)EG(current_execute_data)->function_state.function;

    // get self reference
    ClassBase *meta = data->self->_base;

    // the data structure was allocated by ourselves in the getMethod or 
    // getStaticMethod functions, we no longer need it now
    efree(data);

    // the function could throw an exception
    try
    {
        // wrap the return value
        Value result(return_value, true);

        // construct parameters
        ParametersImpl params(this_ptr, ZEND_NUM_ARGS() TSRMLS_CC);

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
        process(exception TSRMLS_CC);
    }
}

/**
 *  Method that returns the function definition of the __call function
 *  @param  object_ptr
 *  @param  method_name
 *  @param  method_len
 *  @param  tsrm_ls
 *  @return zend_function
 */
#if PHP_VERSION_ID < 50399
zend_function *ClassImpl::getMethod(zval **object_ptr, char *method_name, int method_len TSRMLS_DC)
#else
zend_function *ClassImpl::getMethod(zval **object_ptr, char *method_name, int method_len, const zend_literal *key TSRMLS_DC)
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
    auto *defaultFunction = std_object_handlers.get_method(object_ptr, method_name, method_len TSRMLS_CC);
#else
    auto *defaultFunction = std_object_handlers.get_method(object_ptr, method_name, method_len, key TSRMLS_CC);
#endif

    // did the default implementation do anything?
    if (defaultFunction) return defaultFunction;
    
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(*object_ptr TSRMLS_CC);

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
    function->handler = &ClassImpl::callMethod;
    function->arg_info = nullptr;
    function->num_args = 0;
    function->required_num_args = 0;
    function->scope = entry;
    function->fn_flags = ZEND_ACC_CALL_VIA_HANDLER;
    function->function_name = method_name;
    
    // store pointer to ourselves
    data->self = self(entry);
    
    // done (cast to zend_function* is allowed, because a zend_function is a union
    // that has one member being a zend_internal_function)
    return (zend_function *)data;
}

/**
 *  Method that is called right before a static method call is attempted
 *  @param  entry
 *  @param  method
 *  @param  method_len
 *  @param  tsrm_ls
 *  @return zend_function
 */
zend_function *ClassImpl::getStaticMethod(zend_class_entry *entry, char* method, int method_len TSRMLS_DC)
{
    // first we'll check if the default handler does not have an implementation,
    // in that case the method is probably already implemented as a regular method
#if PHP_VERSION_ID < 50399
    auto *defaultFunction = zend_std_get_static_method(entry, method, method_len TSRMLS_CC);
#else
    auto *defaultFunction = zend_std_get_static_method(entry, method, method_len, nullptr TSRMLS_CC);
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
    function->handler = ClassImpl::callMethod;
    function->arg_info = nullptr;
    function->num_args = 0;
    function->required_num_args = 0;
    function->scope = nullptr;
    function->fn_flags = ZEND_ACC_CALL_VIA_HANDLER;
    function->function_name = method;
    
    // store pointer to ourselves
    data->self = self(entry);
    
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
 *  @param  tsrm_ls
 *  @return int
 */
int ClassImpl::getClosure(zval *object, zend_class_entry **entry_ptr, zend_function **func, zval **object_ptr TSRMLS_DC)
{
    // it is really unbelievable how the Zend engine manages to implement every feature
    // in a complete different manner. You would expect the __invoke() and the
    // __call() functions not to be very different from each other. However, they
    // both have a completely different API. This getClosure method is supposed
    // to fill the function parameter with all information about the invoke()
    // method that is going to get called
    
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(object TSRMLS_CC);

    // just like we did for getMethod(), we're going to dynamically allocate memory
    // with all information about the function
    auto *data = (CallData *)emalloc(sizeof(CallData));
    auto *function = &data->func;
    
    // we're going to set all properties
    function->type = ZEND_INTERNAL_FUNCTION;
    function->module = nullptr;
    function->handler = &ClassImpl::callInvoke;
    function->arg_info = nullptr;
    function->num_args = 0;
    function->required_num_args = 0;
    function->scope = entry;
    function->fn_flags = ZEND_ACC_CALL_VIA_HANDLER;
    function->function_name = nullptr;
    
    // store pointer to ourselves
    data->self = self(entry);
    
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
zend_object_handlers *ClassImpl::objectHandlers()
{
    // already initialized?
    if (_initialized) return &_handlers;
    
    // initialize the handlers
    memcpy(&_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    
    // install custom clone function
    if (!_base->clonable()) _handlers.clone_obj = nullptr;
    else _handlers.clone_obj = &ClassImpl::cloneObject;
    
    // functions for the Countable interface
    _handlers.count_elements = &ClassImpl::countElements;
    
    // functions for the ArrayAccess interface
    _handlers.write_dimension = &ClassImpl::writeDimension;
    _handlers.read_dimension = &ClassImpl::readDimension;
    _handlers.has_dimension = &ClassImpl::hasDimension;
    _handlers.unset_dimension = &ClassImpl::unsetDimension;
    
    // functions for the magic properties handlers (__get, __set, __isset and __unset)
    _handlers.write_property = &ClassImpl::writeProperty;
    _handlers.read_property = &ClassImpl::readProperty;
    _handlers.has_property = &ClassImpl::hasProperty;
    _handlers.unset_property = &ClassImpl::unsetProperty;
    
    // when a method is called (__call and __invoke)
    _handlers.get_method = &ClassImpl::getMethod;
    _handlers.get_closure = &ClassImpl::getClosure;
    
    // handler to cast to a different type
    _handlers.cast_object = &ClassImpl::cast;
    
    // method to compare two objects
    _handlers.compare_objects = &ClassImpl::compare;
    
    // remember that object is now initialized
    _initialized = true;
    
    // done
    return &_handlers;
}

/**
 *  Alternative way to retrieve object handlers, given a class entry
 *  @param  entry
 *  @return zend_object_handlers
 */
zend_object_handlers *ClassImpl::objectHandlers(zend_class_entry *entry)
{
    return self(entry)->objectHandlers();
}

/**
 *  Function to compare two objects
 *  @param  val1
 *  @param  val2
 *  @param  tsrm_ls
 *  @return int
 */
int ClassImpl::compare(zval *val1, zval *val2 TSRMLS_DC)
{
    // prevent exceptions
    try
    {
        // retrieve the class entry linked to this object
        auto *entry = zend_get_class_entry(val1 TSRMLS_CC);

        // other object must be of the same type
        if (entry != zend_get_class_entry(val2 TSRMLS_CC)) throw NotImplemented();

        // we need the C++ class meta-information object
        ClassBase *meta = self(entry)->_base;
        
        // get the base objects
        Base *object1 = ObjectImpl::find(val1 TSRMLS_CC)->object();
        Base *object2 = ObjectImpl::find(val2 TSRMLS_CC)->object();
        
        // run the compare method
        return meta->callCompare(object1, object2);
    }
    catch (const NotImplemented &exception)
    {
        // it was not implemented, do we have a default?
        if (!std_object_handlers.compare_objects) return 1;
        
        // call default
        return std_object_handlers.compare_objects(val1, val2 TSRMLS_CC);
    }
    catch (Exception &exception)
    {
        // a Php::Exception was thrown by the extension __compare function, 
        // pass this on to user space
        process(exception TSRMLS_CC);
        
        // what shall we return here...
        return 1;
    }
}

/**
 *  Function to cast the object to a different type
 *  @param  val
 *  @param  retval
 *  @param  type
 *  @param  tsrm_ls
 *  @return int
 */
int ClassImpl::cast(zval *val, zval *retval, int type TSRMLS_DC)
{
    // get the base c++ object
    Base *object = ObjectImpl::find(val TSRMLS_CC)->object();
    
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(val TSRMLS_CC);

    // we need the C++ class meta-information object
    ClassBase *meta = self(entry)->_base;
    
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
        case Type::Numeric:     result = meta->callToInteger(object).detach();  break;
        case Type::Float:       result = meta->callToFloat(object).detach();    break;
        case Type::Bool:        result = meta->callToBool(object).detach();     break;
        case Type::String:      result = meta->callToString(object).detach();   break;
        default:                throw NotImplemented();                         break;
        }
        
        // @todo do we turn into endless conversion if the __toString object returns 'this' ??
        // (and if it does: who cares? If the extension programmer is stupid, why do we have to suffer?)
        
        // is the original parameter overwritten?
        if (val == retval) zval_dtor(val);
        
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
        return std_object_handlers.cast_object(val, retval, type TSRMLS_CC);
    }
    catch (Exception &exception)
    {
        // pass on the exception to php userspace
        process(exception TSRMLS_CC);
        
        // done
        return FAILURE;
    }
}
    
/**
 *  Function that is called to create space for a cloned object
 *  @param  val                     The object to be cloned
 *  @return zend_obejct_value       The object to be created
 */
zend_object_value ClassImpl::cloneObject(zval *val TSRMLS_DC)
{
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(val TSRMLS_CC);

    // we need the C++ class meta-information object
    ClassImpl *impl = self(entry);
    ClassBase *meta = impl->_base;

    // retrieve the old object, which we are going to copy
    ObjectImpl *old_object = ObjectImpl::find(val TSRMLS_CC);

    // create a new base c++ object
    auto *cpp = meta->clone(old_object->object());
    
    // report error on failure (this does not occur because the cloneObject()
    // method is only installed as handler when we have seen that there is indeed
    // a copy constructor). Because this function is directly called from the
    // Zend engine, we can call zend_error() (which does a longjmp()) to throw
    // an exception back to the Zend engine)
    if (!cpp) zend_error(E_ERROR, "Unable to clone %s", entry->name);

    // the thing we're going to return
    zend_object_value result;
    
    // set the handlers
    result.handlers = impl->objectHandlers();
    
    // store the object
    ObjectImpl *new_object = new ObjectImpl(entry, cpp, 1 TSRMLS_CC);

    // store the object in the object cache
    result.handle = new_object->handle();
    
    // clone the members (this will also call the __clone() function if the user
    // had registered that as a visible method)
    zend_objects_clone_members(new_object->php(), result, old_object->php(), Z_OBJ_HANDLE_P(val) TSRMLS_CC);
    
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
int ClassImpl::countElements(zval *object, long *count TSRMLS_DC)
{
    // does it implement the countable interface?
    Countable *countable = dynamic_cast<Countable*>(ObjectImpl::find(object TSRMLS_CC)->object());

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
            process(exception TSRMLS_CC);
            
            // unreachable
            return FAILURE;
        }
    }
    else
    {
        // Countable interface was not implemented, check if there is a default
        if (!std_object_handlers.count_elements) return FAILURE;
        
        // call default
        return std_object_handlers.count_elements(object, count TSRMLS_CC);
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
 *  @param  tsrm_ls
 *  @return zval
 */
zval *ClassImpl::readDimension(zval *object, zval *offset, int type TSRMLS_DC)
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
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(ObjectImpl::find(object TSRMLS_CC)->object());
    
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
            process(exception TSRMLS_CC);
            
            // unreachable
            return Value(nullptr).detach();
        }
    }
    else
    {
        // ArrayAccess not implemented, check if there is a default handler
        if (!std_object_handlers.read_dimension) return nullptr;
        
        // call default
        return std_object_handlers.read_dimension(object, offset, type TSRMLS_CC);
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
 *  @param  tsrm_ls
 *  @return zval
 */
void ClassImpl::writeDimension(zval *object, zval *offset, zval *value TSRMLS_DC)
{
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(ObjectImpl::find(object TSRMLS_CC)->object());
    
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
            process(exception TSRMLS_CC);
        }
    }
    else
    {
        // ArrayAccess not interface, check if there is a default handler
        if (!std_object_handlers.write_dimension) return;
        
        // call the default
        std_object_handlers.write_dimension(object, offset, value TSRMLS_CC);
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
 *  @param  tsrm_ls
 *  @return bool
 */
int ClassImpl::hasDimension(zval *object, zval *member, int check_empty TSRMLS_DC)
{
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(ObjectImpl::find(object TSRMLS_CC)->object());
    
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
            process(exception TSRMLS_CC);
            
            // unreachable
            return false;
        }
    }
    else
    {
        // ArrayAccess interface is not implemented, check if there is a default handler
        if (!std_object_handlers.has_dimension) return 0;

        // call default
        return std_object_handlers.has_dimension(object, member, check_empty TSRMLS_CC);
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
 *  @param  tsrm_ls
 */
void ClassImpl::unsetDimension(zval *object, zval *member TSRMLS_DC)
{
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(ObjectImpl::find(object TSRMLS_CC)->object());
    
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
            process(exception TSRMLS_CC);
        }
    }
    else
    {
        // ArrayAccess is not implemented, is a default handler available?
        if (!std_object_handlers.unset_dimension) return;
        
        // call the default
        std_object_handlers.unset_dimension(object, member TSRMLS_CC);
    }
}

/**
 *  Helper method to turn a property into a zval
 *  @param  value
 *  @param  type
 *  @return Value
 */
zval *ClassImpl::toZval(Value &&value, int type)
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
 *  @param  tsrm_ls
 *  @return val
 */
#if PHP_VERSION_ID < 50399
zval *ClassImpl::readProperty(zval *object, zval *name, int type TSRMLS_DC)
#else
zval *ClassImpl::readProperty(zval *object, zval *name, int type, const zend_literal *key TSRMLS_DC)
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
    Base *base = ObjectImpl::find(object TSRMLS_CC)->object();
    
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(object TSRMLS_CC);

    // we need the C++ class meta-information object
    ClassImpl *impl = self(entry);
    ClassBase *meta = impl->_base;
    
    // the default implementation throws an exception, so by catching 
    // the exception we know if the object was implemented by the user or not
    try
    {
        // convert name to a Value object
        Value key(name);
        
        // is it a property with a callback?
        auto iter = impl->_properties.find(key);
        
        // was it found?
        if (iter == impl->_properties.end())
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
        return std_object_handlers.read_property(object, name, type TSRMLS_CC);
#else
        return std_object_handlers.read_property(object, name, type, key TSRMLS_CC);
#endif
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method 
        // implementation, send it to user space
        process(exception TSRMLS_CC);
        
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
 *  @param  tsrm_ls
 *  @return zval
 */
#if PHP_VERSION_ID < 50399
void ClassImpl::writeProperty(zval *object, zval *name, zval *value TSRMLS_DC)
#else
void ClassImpl::writeProperty(zval *object, zval *name, zval *value, const zend_literal *key TSRMLS_DC)
#endif
{
    // retrieve the object and class
    Base *base = ObjectImpl::find(object TSRMLS_CC)->object();
    
    // retrieve the class entry linked to this object
    auto *entry = zend_get_class_entry(object TSRMLS_CC);

    // we need the C++ class meta-information object
    ClassImpl *impl = self(entry);
    ClassBase *meta = impl->_base;

    // the default implementation throws an exception, if we catch that
    // we know for sure that the user has not overridden the __set method
    try
    {
        // wrap the name
        Value key(name);
        
        // check if the property has a callback
        auto iter = impl->_properties.find(key);
        
        // is it set?
        if (iter == impl->_properties.end())
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
        std_object_handlers.write_property(object, name, value TSRMLS_CC);
#else
        std_object_handlers.write_property(object, name, value, key TSRMLS_CC);
#endif
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method 
        // implementation, send it to user space
        process(exception TSRMLS_CC);
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
 *  @param  tsrm_ls
 *  @return bool
 */
#if PHP_VERSION_ID < 50399
int ClassImpl::hasProperty(zval *object, zval *name, int has_set_exists TSRMLS_DC)
#else
int ClassImpl::hasProperty(zval *object, zval *name, int has_set_exists, const zend_literal *key TSRMLS_DC)
#endif
{
    // the default implementation throws an exception, if we catch that
    // we know for sure that the user has not overridden the __isset method
    try
    {
        // get the cpp object
        Base *base = ObjectImpl::find(object TSRMLS_CC)->object();
        
        // retrieve the class entry linked to this object
        auto *entry = zend_get_class_entry(object TSRMLS_CC);

        // we need the C++ class meta-information object
        ClassImpl *impl = self(entry);
        ClassBase *meta = impl->_base;
        
        // convert the name to a Value object
        Value key(name);

        // check if this is a callback property
        if (impl->_properties.find(key) != impl->_properties.end()) return true;

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
        return std_object_handlers.has_property(object, name, has_set_exists TSRMLS_CC);
#else
        return std_object_handlers.has_property(object, name, has_set_exists, key TSRMLS_CC);
#endif
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method 
        // implementation, send it to user space
        process(exception TSRMLS_CC);
        
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
 *  @param  tsrm_ls
 */
#if PHP_VERSION_ID < 50399
void ClassImpl::unsetProperty(zval *object, zval *member TSRMLS_DC)
#else
void ClassImpl::unsetProperty(zval *object, zval *member, const zend_literal *key TSRMLS_DC)
#endif
{
    // the default implementation throws an exception, if we catch that
    // we know for sure that the user has not overridden the __unset method
    try
    {
        // retrieve the class entry linked to this object
        auto *entry = zend_get_class_entry(object TSRMLS_CC);

        // we need the C++ class meta-information object
        ClassImpl *impl = self(entry);
        
        // property name
        Value name(member);
        
        // is this a callback property?
        auto iter = impl->_properties.find(name);
        
        // if the property does not exist, we forward to the __unset
        if (iter == impl->_properties.end()) impl->_base->callUnset(ObjectImpl::find(object TSRMLS_CC)->object(), member);
        
        // callback properties cannot be unset
        zend_error(E_ERROR, "Property %s can not be unset", (const char *)name);
    }
    catch (const NotImplemented &exception)
    {
        // __unset was not implemented, do we have a default?
        if (!std_object_handlers.unset_property) return;
        
        // call the default
#if PHP_VERSION_ID < 50399
        std_object_handlers.unset_property(object, member TSRMLS_CC);
#else
        std_object_handlers.unset_property(object, member, key TSRMLS_CC);
#endif
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method 
        // implementation, send it to user space
        process(exception TSRMLS_CC);
    }
}

/**
 *  Function that is called when an object is about to be destructed
 *  This will call the magic __destruct method
 *  @param  object
 *  @param  handle
 *  @param  tsrm_ls
 */
void ClassImpl::destructObject(zend_object *object, zend_object_handle handle TSRMLS_DC)
{
    // find object
    ObjectImpl *obj = ObjectImpl::find(object);
    
    // get meta info
    ClassImpl *impl = self(object->ce);
    
    // prevent exceptions
    try
    {
        // call the destruct function
        if (obj->object()) impl->_base->callDestruct(obj->object());
    }
    catch (const NotImplemented &exception)
    {
        // fallback on the default destructor call
        zend_objects_destroy_object(object, handle TSRMLS_CC);
    }
    catch (Exception &exception)
    {
        // a regular Php::Exception was thrown by the extension, pass it on
        // to PHP user space
        process(exception TSRMLS_CC);
    }
}

/**
 *  Function that is called to clean up space that is occupied by the object
 *  @param  object      The object to be deallocated
 *  @param  tsrm_ls
 */
void ClassImpl::freeObject(zend_object *object TSRMLS_DC)
{
    // allocate memory for the object
    ObjectImpl *obj = ObjectImpl::find(object);
    
    // no longer need it
    obj->destruct(TSRMLS_C);
}

/**
 *  Function that is called when an instance of the class needs to be created.
 *  This function will create the C++ class, and the PHP object
 *  @param  entry                   Pointer to the class information
 *  @param  tsrm_ls
 *  @return zend_object_value       The newly created object
 */
zend_object_value ClassImpl::createObject(zend_class_entry *entry TSRMLS_DC)
{
    // we need the C++ class meta-information object
    ClassImpl *impl = self(entry);

    // create a new base C++ object
    auto *cpp = impl->_base->construct();

    // report error on failure, because this function is called directly from the
    // Zend engine, we can call zend_error() here (which does a longjmp() back to
    // the Zend engine)
    if (!cpp) zend_error(E_ERROR, "Unable to instantiate %s", entry->name);

    // the thing we're going to return
    zend_object_value result;
    
    // set the handlers
    result.handlers = impl->objectHandlers();
    
    // create the object in the zend engine
    ObjectImpl *object = new ObjectImpl(entry, cpp, 1 TSRMLS_CC);
    
    // store the object in the object cache
    result.handle = object->handle();
    
    // done
    return result;
}

/**
 *  Function to create a new iterator to iterate over an object
 *  @param  entry                   The class entry
 *  @param  object                  The object to iterate over
 *  @param  by_ref                  ?????
 *  @param  tsrm_ls
 *  @return zend_object_iterator*   Pointer to the iterator
 */
zend_object_iterator *ClassImpl::getIterator(zend_class_entry *entry, zval *object, int by_ref TSRMLS_DC)
{
    // by-ref is not possible (copied from SPL), this function is called directly
    // from the Zend engine, so we can use zend_error() to longjmp() back to the 
    // Zend engine)
    if (by_ref) zend_error(E_ERROR, "Foreach by ref is not possible");
    
    // retrieve the traversable object
    Traversable *traversable = dynamic_cast<Traversable*>(ObjectImpl::find(object TSRMLS_CC)->object());
    
    // user may throw an exception in the getIterator() function
    try
    {
        // create an iterator
        auto *iterator = new IteratorImpl(traversable->getIterator());
        
        // return the implementation
        return iterator->implementation();
    }
    catch (Exception &exception)
    {
        // user threw an exception in its method 
        // implementation, send it to user space
        process(exception TSRMLS_CC);
        
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
 *  @param  tsrm_ls
 *  @return int
 */
int ClassImpl::serialize(zval *object, unsigned char **buffer, zend_uint *buf_len, zend_serialize_data *data TSRMLS_DC)
{
    // get the serializable object
    Serializable *serializable = dynamic_cast<Serializable*>(ObjectImpl::find(object TSRMLS_CC)->object());
    
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
 *  @param  tsrm_ls
 *  @return int
 */
int ClassImpl::unserialize(zval **object, zend_class_entry *entry, const unsigned char *buffer, zend_uint buf_len, zend_unserialize_data *data TSRMLS_DC)
{
    // create the PHP object
    object_init_ex(*object, entry);
    
    // turn this into a serializale
    Serializable *serializable = dynamic_cast<Serializable*>(ObjectImpl::find(*object TSRMLS_CC)->object());
    
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
const struct _zend_function_entry *ClassImpl::entries()
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
 *  @param  base        the c++ class object created in the extension
 *  @param  prefix      namespace prefix
 *  @param  tsrm_ls
 *  @return zend_class_entry
 */
zend_class_entry *ClassImpl::initialize(ClassBase *base, const std::string &prefix TSRMLS_DC)
{
    // store base pointer
    _base = base;
    
    // the class entry
    zend_class_entry entry;

    // update the name
    if (prefix.size() > 0) _name = prefix + "\\" + _name;

    // initialize the class entry
    INIT_CLASS_ENTRY_EX(entry, _name.c_str(), _name.size(), entries());

    // we need a special constructor
    entry.create_object = &ClassImpl::createObject;
    
    // register function that is called for static method calls
    entry.get_static_method = &ClassImpl::getStaticMethod;
    
    // for traversable classes we install a special method to get the iterator
    if (_base->traversable()) entry.get_iterator = &ClassImpl::getIterator;
    
    // for serializable classes, we install callbacks for serializing and unserializing
    if (_base->serializable())
    {
        // add handlers to serialize and unserialize
        entry.serialize = &ClassImpl::serialize;
        entry.unserialize = &ClassImpl::unserialize;
    }
    
    // do we have a base class?
    if (_parent) 
    {
        // check if the base class was already defined
        if (_parent->_entry)
        {
            // register the class
            _entry = zend_register_internal_class_ex(&entry, _parent->_entry, const_cast<char*>(_parent->name().c_str()) TSRMLS_CC);
        }
        else
        {
            // report an error - the extension programmer probably made an error
            std::cerr << "Derived class " << name() << " is initialized before base class " << _parent->name() << ": base class is ignored" << std::endl;
            
            // register the class, but without the base class
            _entry = zend_register_internal_class(&entry TSRMLS_CC);
        }
    }
    else
    {
        // register the class
        _entry = zend_register_internal_class(&entry TSRMLS_CC);
    }
    
    // register the classes
    for (auto &interface : _interfaces)
    {
        // register this interface
        if (interface->_entry) zend_class_implements(_entry TSRMLS_CC, 1, interface->_entry);
        
        // otherwise report an error
        else std::cerr << "Derived class " << name() << " is initialized before base class " << interface->name() << ": interface is ignored" << std::endl;
    }

    // this pointer has to be copied to temporary pointer, as &this causes compiler error
    ClassImpl *impl = this;

#if PHP_VERSION_ID >= 50400

    // allocate doc comment to contain an empty string + a hidden pointer
    char *_comment = (char *)malloc(1 + sizeof(ClassImpl *));

    // empty string on first position
    _comment[0] = '\0';

    // copy the 'this' pointer to the doc-comment
    memcpy(_comment+1, &impl, sizeof(ClassImpl *));

    // set our comment in the actual class entry
    _entry->info.user.doc_comment = _comment;

#else

    // Reallocate some extra space in the name in the zend_class_entry so we can fit a pointer behind it
    _entry->name = (char *) realloc(_entry->name, _entry->name_length + 1 + sizeof(ClassImpl *));

    // Copy the pointer after it
    memcpy(_entry->name + _entry->name_length + 1, &impl, sizeof(ClassImpl *));

#endif

    // set access types flags for class
    _entry->ce_flags = (int)_type;

    // declare all member variables
    for (auto &member : _members) member->initialize(_entry TSRMLS_CC);
    
    // done
    return _entry;
}

/**
 *  End namespace
 */
}

