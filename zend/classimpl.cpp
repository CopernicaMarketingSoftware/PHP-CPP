/**
 *  ClassImpl.cpp
 *
 *  Implementation file for the ClassImpl class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"
#include <cstring>

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
    delete[] _entries;

    // free the stored pointer
    if (_self) zend_string_release(_self);
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
    /**
     *  somebody could have extended this class from PHP userland, in which
     *  case trying to dereference the doc_comment would result in a disaster
     *  because it does not point to a class implemented by PHP-CPP at all!
     *
     *  if this happens we need to keep going until we find the object that
     *  was implemented by us. For this we are going to make the assumption
     *  that we are the only ones misusing the doc_comment the way we do.
     *
     *  Usually the doc_comment is not set (it equals the nullptr) and if it
     *  is set, the accompanying doc_comment_len should be non-zero to
     *  indicate the number of characters in it.
     *
     *  When, however, we use the doc_comment from inside PHP-CPP to store
     *  the classimpl, we store a null-character (to immediately terminate
     *  the string, in case PHP tries to read it) and after that the pointer
     *  and we leave the doc_comment_len at 0.
     */
    while (entry->parent && (entry->info.user.doc_comment == nullptr || ZSTR_LEN(entry->info.user.doc_comment) > 0))
    {
        // we did not create this class entry, but luckily we have a parent
        entry = entry->parent;
    }

    // retrieve the comment (it has a pointer hidden in it to the ClassBase object)
    const char *comment = ZSTR_VAL(entry->info.user.doc_comment);

    // the first byte of the comment is an empty string (null character), but
    // the next bytes contain a pointer to the ClassBase class
    return *((ClassImpl **)(comment + 1));
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
    auto *data = (CallData *)execute_data->func;
    zend_internal_function *func = &data->func;

    // retrieve the function name
    const char *name = ZSTR_VAL(func->function_name);
    ClassBase *meta = data->self->_base;

    // the data structure was allocated by ourselves in the getMethod or
    // getStaticMethod functions, we no longer need it when the function falls
    // out of scope
    DelayedFree df(data);

    // the function could throw an exception
    try
    {
        // wrap the return value
        Value result(return_value, true);

        // construct parameters
        ParametersImpl params(getThis(), ZEND_NUM_ARGS());

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
        process(exception);
    }
}

/**
 *  Handler function that runs the __invoke function
 *  @param  ...     All normal parameters for function calls
 */
void ClassImpl::callInvoke(INTERNAL_FUNCTION_PARAMETERS)
{
    // retrieve the originally called (and by us allocated) function object
    auto *data = (CallData *)execute_data->func;

    // get self reference
    ClassBase *meta = data->self->_base;

    // the data structure was allocated by ourselves in the getMethod or
    // getStaticMethod functions, we no longer need it when the function falls
    // out of scope
    DelayedFree df(data);

    // the function could throw an exception
    try
    {
        // wrap the return value
        Value result(return_value, true);

        // construct parameters
        ParametersImpl params(getThis(), ZEND_NUM_ARGS());

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
        process(exception);
    }
}

/**
 *  Method that returns the function definition of the __call function
 *
 *  @param  object      Pointer to the object from which we want to retrieve the member function
 *  @param  method      The method that we want information about
 *  @param  key         ???
 *  @return zend_function
 */
zend_function *ClassImpl::getMethod(zend_object **object, zend_string *method, const zval *key)
{
    // something strange about the Zend engine (once more). The structure with
    // object-handlers has a get_method and call_method member. When a function is
    // called, the get_method function is called first, to retrieve information
    // about the method (like the handler that should be called to execute it),
    // after that, this returned handler is also called. The call_method property
    // of the object_handlers structure however, never gets called. Typical.

    // first we'll check if the default handler does not have an implementation,
    // in that case the method is probably already implemented as a regular method
    auto *defaultFunction = std_object_handlers.get_method(object, method, key);

    // did the default implementation do anything?
    if (defaultFunction) return defaultFunction;

    // retrieve the class entry linked to this object
    auto *entry = (*object)->ce;

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
    function->function_name = method;

    // store pointer to ourselves
    data->self = self(entry);

    // done (cast to zend_function* is allowed, because a zend_function is a union
    // that has one member being a zend_internal_function)
    return (zend_function *)data;
}

/**
 *  Method that is called right before a static method call is attempted
 *
 *  @param  entry       The class entry to find the static function in
 *  @param  method      The method to get information about
 *  @param  key         ???
 *  @return zend_function
 */
zend_function *ClassImpl::getStaticMethod(zend_class_entry *entry, zend_string *method)
{
    // first we'll check if the default handler does not have an implementation,
    // in that case the method is probably already implemented as a regular method
    auto *defaultFunction = zend_std_get_static_method(entry, method, nullptr);

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
 *  @return int
 */
int ClassImpl::getClosure(zval *object, zend_class_entry **entry_ptr, zend_function **func, zend_object **object_ptr)
{
    // it is really unbelievable how the Zend engine manages to implement every feature
    // in a complete different manner. You would expect the __invoke() and the
    // __call() functions not to be very different from each other. However, they
    // both have a completely different API. This getClosure method is supposed
    // to fill the function parameter with all information about the invoke()
    // method that is going to get called

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
    function->scope = *entry_ptr;
    function->fn_flags = ZEND_ACC_CALL_VIA_HANDLER;
    function->function_name = nullptr;

    // store pointer to ourselves (note that the entry_ptr is useless
    // inside this function as it is always uninitialized for some reason)
    data->self = self(Z_OBJCE_P(object));

    // assign this dynamically allocated variable to the func parameter
    // the cast is ok, because zend_internal_function is a member of the
    // zend_function union
    *func = (zend_function *)data;

    // the object_ptr should be filled with the object on which the method is
    // called (otherwise the Zend engine tries to call the method statically)
    *object_ptr = Z_OBJ_P(object);

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

    // register destructor and deallocator
    _handlers.dtor_obj = &ClassImpl::destructObject;
    _handlers.free_obj = &ClassImpl::freeObject;

    // handler to cast to a different type
    _handlers.cast_object = &ClassImpl::cast;

    // method to compare two objects
    _handlers.compare_objects = &ClassImpl::compare;

    // set the offset between our class implementation and
    // the zend_object member in the allocated structure
    _handlers.offset = ObjectImpl::offset();

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
 *  @return int
 */
int ClassImpl::compare(zval *val1, zval *val2)
{
    // prevent exceptions
    try
    {
        // retrieve the class entry linked to this object
        auto *entry = Z_OBJCE_P(val1);

        // other object must be of the same type
        if (entry != Z_OBJCE_P(val2)) throw NotImplemented();

        // we need the C++ class meta-information object
        ClassBase *meta = self(entry)->_base;

        // get the base objects
        Base *object1 = ObjectImpl::find(val1)->object();
        Base *object2 = ObjectImpl::find(val2)->object();

        // run the compare method
        return meta->callCompare(object1, object2);
    }
    catch (const NotImplemented &exception)
    {
        // it was not implemented, do we have a default?
        if (!std_object_handlers.compare_objects) return 1;

        // call default
        return std_object_handlers.compare_objects(val1, val2);
    }
    catch (Exception &exception)
    {
        // a Php::Exception was thrown by the extension __compare function,
        // pass this on to user space
        process(exception);

        // what shall we return here...
        return 1;
    }
}

/**
 *  Function to cast the object to a different type
 *  @param  val
 *  @param  retval
 *  @param  type
 *  @return int
 */
int ClassImpl::cast(zval *val, zval *retval, int type)
{
    // get the base c++ object
    Base *object = ObjectImpl::find(val)->object();

    // retrieve the class entry linked to this object
    auto *entry = Z_OBJCE_P(val);

    // we need the C++ class meta-information object
    ClassBase *meta = self(entry)->_base;

    // when the magic function it not implemented, an exception will be thrown,
    // and the extension may throw a Php::Exception
    try
    {
        // the result value
        Value result;

        // check type
        switch ((Type)type) {
            case Type::Numeric:     result = meta->callToInteger(object);   break;
            case Type::Float:       result = meta->callToFloat(object);     break;
            case Type::Bool:        result = meta->callToBool(object);      break;
            case Type::String:      result = meta->callToString(object);    break;
            default:                throw NotImplemented();                 break;
        }

        // @todo do we turn into endless conversion if the __toString object returns 'this' ??
        // (and if it does: who cares? If the extension programmer is stupid, why do we have to suffer?)

        // overwrite the result
        ZVAL_DUP(retval, result._val);

        // done
        return SUCCESS;
    }
    catch (const NotImplemented &exception)
    {
        // is there a default?
        if (!std_object_handlers.cast_object) return FAILURE;

        // call default
        return std_object_handlers.cast_object(val, retval, type);
    }
    catch (Exception &exception)
    {
        // pass on the exception to php userspace
        process(exception);

        // done
        return FAILURE;
    }
}

/**
 *  Function that is called to create space for a cloned object
 *
 *  @param  val             The object to be cloned
 *  @return zend_object     The object to be created
 */
zend_object *ClassImpl::cloneObject(zval *val)
{
    // retrieve the class entry linked to this object
    auto *entry = Z_OBJCE_P(val);

    // we need the C++ class meta-information object
    ClassImpl *impl = self(entry);
    ClassBase *meta = impl->_base;

    // retrieve the old object, which we are going to copy
    ObjectImpl *old_object = ObjectImpl::find(val);

    // create a new base c++ object
    auto *cpp = meta->clone(old_object->object());

    // report error on failure (this does not occur because the cloneObject()
    // method is only installed as handler when we have seen that there is indeed
    // a copy constructor). Because this function is directly called from the
    // Zend engine, we can call zend_error() (which does a longjmp()) to throw
    // an exception back to the Zend engine)
    if (!cpp) zend_error(E_ERROR, "Unable to clone %s", entry->name);

    // store the object
    auto *new_object = new ObjectImpl(entry, cpp, impl->objectHandlers(), 1);

    // clone the members (this will also call the __clone() function if the user
    // had registered that as a visible method)
    zend_objects_clone_members(new_object->php(), old_object->php());

    // was a custom clone method installed? If not we call the magic c++ __clone method
    if (!entry->clone) meta->callClone(cpp);

    // done
    return new_object->php();
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
int ClassImpl::countElements(zval *object, zend_long *count)
{
    // does it implement the countable interface?
    Countable *countable = dynamic_cast<Countable*>(ObjectImpl::find(object)->object());

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
            process(exception);

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
 *  @param  rv              Pointer to where to store the data
 *  @return zval
 */
zval *ClassImpl::readDimension(zval *object, zval *offset, int type, zval *rv)
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
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(ObjectImpl::find(object)->object());

    // if it does not implement the ArrayAccess interface, we rely on the default implementation
    if (arrayaccess)
    {
        // the C++ code may throw an exception
        try
        {
            // ArrayAccess is implemented, call function
            return toZval(arrayaccess->offsetGet(offset), type, rv);
        }
        catch (Exception &exception)
        {
            // process the exception (send it to user space)
            process(exception);

            // unreachable
            return Value(nullptr).detach(false);
        }
    }
    else
    {
        // ArrayAccess not implemented, check if there is a default handler
        if (!std_object_handlers.read_dimension) return nullptr;

        // call default
        return std_object_handlers.read_dimension(object, offset, type, rv);
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
void ClassImpl::writeDimension(zval *object, zval *offset, zval *value)
{
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(ObjectImpl::find(object)->object());

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
            process(exception);
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
int ClassImpl::hasDimension(zval *object, zval *member, int check_empty)
{
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(ObjectImpl::find(object)->object());

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
            process(exception);

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
void ClassImpl::unsetDimension(zval *object, zval *member)
{
    // does it implement the arrayaccess interface?
    ArrayAccess *arrayaccess = dynamic_cast<ArrayAccess*>(ObjectImpl::find(object)->object());

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
            process(exception);
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
 *
 *  @param  value   The value to convert to a zval
 *  @param  type    The type of operation (read or write)
 *  @param  rv      Pointer to where to store the data
 *  @return The result (same as the ptr input)
 */
zval *ClassImpl::toZval(Value &&value, int type, zval *rv)
{
    // the result zval that needs to be copied over
    Zval result;

    /**
     *  Because we do not want the value object to destruct the zval when
     *  it falls out of scope, we detach the zval from it, if this is a regular
     *  read operation we can do this right away.
     *
     *  For write operations we need to check the refcount. If the refcount is
     *  only 1 (meaning the value object has the only reference) we cannot return
     *  a reference because there _is_ nothing to reference (the value will destruct)
     */
    if (type == 0 || value.refcount() <= 1)
    {
        // first retrieve the value so we can copy it
        result = value.detach(true);
    }
    // this is an editable zval, return a reference to it
    else
    {
        // we're dealing with an editable zval, retrieve a reference variable
        result = Value(value.detach(false), true).detach(true);
    }

    // now copy the value over to the pointer
    ZVAL_COPY_VALUE(rv, result);

    // return the pointer to the value
    return rv;
}

/**
 *  Function that is called when a property is read
 *
 *  @param  object          The object on which it is called
 *  @param  offset          The name of the property
 *  @param  type            The type of the variable???
 *  @param  cache_slot      The cache slot used
 *  @param  rv              Pointer to where to store the data
 *  @return val
 */
zval *ClassImpl::readProperty(zval *object, zval *name, int type, void **cache_slot, zval *rv)
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
    Base *base = ObjectImpl::find(object)->object();

    // retrieve the class entry linked to this object
    auto *entry = Z_OBJCE_P(object);

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
            return toZval(meta->callGet(base, key), type, rv);
        }
        else
        {
            // get the value
            return toZval(iter->second->get(base), type, rv);
        }
    }
    catch (const NotImplemented &exception)
    {
        // __get() function was not overridden by the user
        if (!std_object_handlers.read_property) return nullptr;

        // call default
        return std_object_handlers.read_property(object, name, type, cache_slot, rv);
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method
        // implementation, send it to user space
        process(exception);

        // unreachable
        return Value(nullptr).detach(false);
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
 *  @param  cache_slot      The cache slot used
 *  @return zval
 */
void ClassImpl::writeProperty(zval *object, zval *name, zval *value, void **cache_slot)
{
    // retrieve the object and class
    Base *base = ObjectImpl::find(object)->object();

    // retrieve the class entry linked to this object
    auto *entry = Z_OBJCE_P(object);

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
        std_object_handlers.write_property(object, name, value, cache_slot);
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method
        // implementation, send it to user space
        process(exception);
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
 *  @param  cache_slot      The cache slot used
 *  @return bool
 */
int ClassImpl::hasProperty(zval *object, zval *name, int has_set_exists, void **cache_slot)
{
    // the default implementation throws an exception, if we catch that
    // we know for sure that the user has not overridden the __isset method
    try
    {
        // get the cpp object
        Base *base = ObjectImpl::find(object)->object();

        // retrieve the class entry linked to this object
        auto *entry = Z_OBJCE_P(object);

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
        return std_object_handlers.has_property(object, name, has_set_exists, cache_slot);
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method
        // implementation, send it to user space
        process(exception);

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
 *  @param  cache_slot      The cache slot used
 */
void ClassImpl::unsetProperty(zval *object, zval *member, void **cache_slot)
{
    // the default implementation throws an exception, if we catch that
    // we know for sure that the user has not overridden the __unset method
    try
    {
        // retrieve the class entry linked to this object
        auto *entry = Z_OBJCE_P(object);

        // we need the C++ class meta-information object
        ClassImpl *impl = self(entry);

        // property name
        Value name(member);

        // is this a callback property?
        auto iter = impl->_properties.find(name);

        // if the property does not exist, we forward to the __unset
        if (iter == impl->_properties.end()) impl->_base->callUnset(ObjectImpl::find(object)->object(), member);

        // callback properties cannot be unset
        zend_error(E_ERROR, "Property %s can not be unset", (const char *)name);
    }
    catch (const NotImplemented &exception)
    {
        // __unset was not implemented, do we have a default?
        if (!std_object_handlers.unset_property) return;

        // call the default
        std_object_handlers.unset_property(object, member, cache_slot);
    }
    catch (Exception &exception)
    {
        // user threw an exception in its magic method
        // implementation, send it to user space
        process(exception);
    }
}

/**
 *  Function that is called when an object is about to be destructed
 *  This will call the magic __destruct method
 *  @param  object
 */
void ClassImpl::destructObject(zend_object *object)
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
        zend_objects_destroy_object(object);
    }
    catch (Exception &exception)
    {
        // a regular Php::Exception was thrown by the extension, pass it on
        // to PHP user space
        process(exception);
    }
}

/**
 *  Function that is called to clean up space that is occupied by the object
 *  @param  object      The object to be deallocated
 */
void ClassImpl::freeObject(zend_object *object)
{
    // allocate memory for the object
    ObjectImpl *obj = ObjectImpl::find(object);

    // no longer need it
    obj->destruct();
}

/**
 *  Function that is called when an instance of the class needs to be created.
 *  This function will create the C++ class, and the PHP object
 *  @param  entry                   Pointer to the class information
 *  @return zend_object_value       The newly created object
 */
zend_object *ClassImpl::createObject(zend_class_entry *entry)
{
    // we need the C++ class meta-information object
    ClassImpl *impl = self(entry);

    // create a new base C++ object
    auto *cpp = impl->_base->construct();

    // report error on failure, because this function is called directly from the
    // Zend engine, we can call zend_error() here (which does a longjmp() back to
    // the Zend engine)
    if (!cpp) zend_error(E_ERROR, "Unable to instantiate %s", entry->name);

    // create the object in the zend engine
    auto *object = new ObjectImpl(entry, cpp, impl->objectHandlers(), 1);

    // return the php object stored in the implementation
    return object->php();
}

/**
 *  Function to create a new iterator to iterate over an object
 *  @param  entry                   The class entry
 *  @param  object                  The object to iterate over
 *  @param  by_ref                  ?????
 *  @return zend_object_iterator*   Pointer to the iterator
 */
zend_object_iterator *ClassImpl::getIterator(zend_class_entry *entry, zval *object, int by_ref)
{
    // by-ref is not possible (copied from SPL), this function is called directly
    // from the Zend engine, so we can use zend_error() to longjmp() back to the
    // Zend engine)
    if (by_ref) zend_error(E_ERROR, "Foreach by ref is not possible");

    // retrieve the traversable object
    Traversable *traversable = dynamic_cast<Traversable*>(ObjectImpl::find(object)->object());
    
    // use might throw an exception in the getIterator() function
    try
    {
        // get userspace iterator
        auto *userspace = traversable->getIterator();

        // we are going to allocate an extended iterator (because php nowadays destructs
        // the iteraters itself, we can no longer let c++ allocate the buffer + object
        // directly, so we first allocate the buffer, which is going to be cleaned up by php)
        auto *buffer = emalloc(sizeof(IteratorImpl));
    
        // and then we use placement-new to allocate the implementation
        auto *wrapper = new(buffer)IteratorImpl(object, userspace);
        
        // done
        return wrapper->implementation();
    }
    catch (Exception &exception)
    {
        // user threw an exception in its method, send it to user space
        process(exception);

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
int ClassImpl::serialize(zval *object, unsigned char **buffer, size_t *buf_len, zend_serialize_data *data)
{
    // get the serializable object
    Serializable *serializable = dynamic_cast<Serializable*>(ObjectImpl::find(object)->object());

    // user may throw an exception in the serialize() function
    try
    {
        // call the serialize method on the object
        auto value = serializable->serialize();

        // allocate the buffer, and copy the data into it (the zend engine will
        // (hopefully) clean up the data for us - the default serialize method does
        // it like this too)
        *buffer = (unsigned char*)estrndup(value.c_str(), value.size());
        *buf_len = value.size();
    }
    catch (Exception &exception)
    {
        // user threw an exception in its method
        // implementation, send it to user space
        process(exception);

        // unreachable
        return FAILURE;
    }

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
int ClassImpl::unserialize(zval *object, zend_class_entry *entry, const unsigned char *buffer, size_t buf_len, zend_unserialize_data *data)
{
    // create the PHP object
    object_init_ex(object, entry);

    // turn this into a serializale
    Serializable *serializable = dynamic_cast<Serializable*>(ObjectImpl::find(object)->object());

    // user may throw an exception in the serialize() function
    try
    {
        // call the unserialize method on it
        serializable->unserialize((const char *)buffer, buf_len);
    }
    catch (Exception &exception)
    {
        // user threw an exception in its method
        // implementation, send it to user space
        //process(exception);
        php_error_docref(NULL, E_NOTICE, "Error while unserializing");

        // unreachable
        return FAILURE;
    }

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
    memset(last, 0, sizeof(*last));

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
 *  @return zend_class_entry
 */
zend_class_entry *ClassImpl::initialize(ClassBase *base, const std::string &prefix)
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
    if (_base->traversable()) 
    {
        // install iterator functions
        entry.get_iterator = &ClassImpl::getIterator;

        // prior to 7.3, the iterator functions were statically allocated.
#if PHP_VERSION_ID < 70300
        entry.iterator_funcs.funcs = IteratorImpl::functions();
#else
        // from 7.3 and up, we may have to allocate it ourself
        //entry.iterator_funcs_ptr = calloc(1, sizeof(zend_class_iterator_funcs));
#endif
    }

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
            _entry = zend_register_internal_class_ex(&entry, _parent->_entry);
        }
        else
        {
            // report an error - the extension programmer probably made an error
            std::cerr << "Derived class " << name() << " is initialized before base class " << _parent->name() << ": base class is ignored" << std::endl;

            // register the class, but without the base class
            _entry = zend_register_internal_class(&entry);
        }
    }
    else
    {
        // register the class
        _entry = zend_register_internal_class(&entry);
    }

    // register the classes
    for (auto &interface : _interfaces)
    {
        // register this interface
        if (interface->_entry) zend_class_implements(_entry, 1, interface->_entry);

        // otherwise report an error
        else std::cerr << "Derived class " << name() << " is initialized before base class " << interface->name() << ": interface is ignored" << std::endl;
    }

    // this pointer has to be copied to temporary pointer, as &this causes compiler error
    ClassImpl *impl = this;

    // allocate memory for the doc_comment (which we abuse for storing a pointer to ourselves)
    _self = zend_string_alloc(sizeof(this), 1);

    // make the string appear empty
    ZSTR_VAL(_self)[0] = '\0';
    ZSTR_LEN(_self) = 0;

    // copy over the 'this'-pointer after the null-character
    std::memcpy(ZSTR_VAL(_self) + 1, &impl, sizeof(impl));

    // install the doc_comment
    _entry->info.user.doc_comment = _self;

    // set access types flags for class
    _entry->ce_flags = (int)_type;

    // declare all member variables
    for (auto &member : _members) member->initialize(_entry);

    // done
    return _entry;
}

/**
 *  End namespace
 */
}

