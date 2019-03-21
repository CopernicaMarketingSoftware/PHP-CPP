/**
 *  callable.h
 *
 *  A wrapper to handle a callback coming from
 *  within PHP
 *
 *  @author Martijn Otto <martijn.otto@copernica.com>
 *  @copyright 2016 - 2019 Copernica B.V.
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  Forward declarations
 */
struct _zend_execute_data;
struct _zval_struct;

/**
 *  Start namespace
 */
namespace Php {

/**
 *  Callback wrapper class
 */
class PHPCPP_EXPORT ZendCallable
{
private:
    /**
     *  Retrieve pointer to the base of the object implementation
     *
     *  @param  execute_data    The current execution scope
     *  @return Pointer to base
     */
    static Base *instance(struct _zend_execute_data *execute_data);

    /**
     *  Check whether we have received valid parameters
     *
     *  If this function returns false a warning will have been
     *  generated and the return value has been set to NULL.
     *
     *  @param  execute_data    The current execution scope
     *  @param  return_value    The return value to set on failure
     */
    static bool valid(struct _zend_execute_data *execute_data, struct _zval_struct *return_value);

    /**
     *  Retrieve the input parameters for the function
     *
     *  @param  execute_data    The current execution scope
     *  @return The input parameters
     */
    static Parameters parameters(struct _zend_execute_data *execute_data);

    /**
     *  Handle throwables
     *  @param  throwable       The object to handle
     */
    static void handle(Throwable &throwable);

    /**
     *  Yield (return) the given value
     *
     *  @param  return_value    The return_value to set
     *  @param  value           The value to return to PHP
     */
    static void yield(struct _zval_struct *return_value, std::nullptr_t value);
    static void yield(struct _zval_struct *return_value, const Php::Value &value);
public:
    /**
     *  Execute the callback
     *
     *  @param  execute_data    Data about the PHP call stack
     *  @param  return_value    The value we are returning to PHP
     */
    template <typename T, void (T::*callback)()>
    static void invoke(struct _zend_execute_data *execute_data, struct _zval_struct *return_value)
    {
        // catch exceptions thrown by the C++ methods
        try
        {
            // cast the base to the correct object and invoke the member
            (static_cast<T*>(instance(execute_data))->*callback)();
            
            // there is no return value, so we just return null
            yield(return_value, nullptr);
        }
        catch (Throwable &throwable)
        {
            // handle the throwable
            handle(throwable);
        }
    }

    /**
     *  Execute the callback
     *
     *  @param  execute_data    Data about the PHP call stack
     *  @param  return_value    The value we are returning to PHP
     */
    template <typename T, void (T::*callback)() const>
    static void invoke(struct _zend_execute_data *execute_data, struct _zval_struct *return_value)
    {
        // catch exceptions thrown by the C++ methods
        try
        {
            // cast the base to the correct object and invoke the member
            (static_cast<T*>(instance(execute_data))->*callback)();

            // there is no return value, so we just return null
            yield(return_value, nullptr);
        }
        catch (Throwable &throwable)
        {
            // handle the exception
            handle(throwable);
        }
    }

    /**
     *  Execute the callback
     *
     *  @param  execute_data    Data about the PHP call stack
     *  @param  return_value    The value we are returning to PHP
     */
    template <typename T, Value (T::*callback)()>
    static void invoke(struct _zend_execute_data *execute_data, struct _zval_struct *return_value)
    {
        // catch exceptions thrown by the C++ methods
        try
        {
            // cast the base to the correct object and invoke the member
            auto result = (static_cast<T*>(instance(execute_data))->*callback)();

            // store the return value in the return_value
            yield(return_value, result);
        }
        catch (Throwable &throwable)
        {
            // handle the exception
            handle(throwable);
        }
    }

    /**
     *  Execute the callback
     *
     *  @param  execute_data    Data about the PHP call stack
     *  @param  return_value    The value we are returning to PHP
     */
    template <typename T, Value (T::*callback)() const>
    static void invoke(struct _zend_execute_data *execute_data, struct _zval_struct *return_value)
    {
        // catch exceptions thrown by the C++ methods
        try
        {
            // cast the base to the correct object and invoke the member
            auto result = (static_cast<T*>(instance(execute_data))->*callback)();

            // store the return value in the return_value
            yield(return_value, result);
        }
        catch (Throwable &throwable)
        {
            // handle the exception
            handle(throwable);
        }
    }

    /**
     *  Execute the callback
     *
     *  @param  execute_data    data about the PHP call stack
     *  @param  return_value    The value we are returning to PHP
     */
    template <typename T, void(T::*callback)(Parameters &parameters)>
    static void invoke(struct _zend_execute_data *execute_data, struct _zval_struct *return_value)
    {
        // check parameter count
        if (!valid(execute_data, return_value)) return;

        // retrieve the parameters
        auto params = parameters(execute_data);

        // catch exceptions thrown by the C++ methods
        try
        {
            // cast the base to the correct object and invoke the member
            (static_cast<T*>(instance(execute_data))->*callback)(params);

            // there is no return value, so we just reutrn null
            yield(return_value, nullptr);
        }
        catch (Throwable &throwable)
        {
            // handle the exception
            handle(throwable);
        }
    }

    /**
     *  Execute the callback
     *
     *  @param  execute_data    data about the PHP call stack
     *  @param  return_value    The value we are returning to PHP
     */
    template <typename T, void(T::*callback)(Parameters &parameters) const>
    static void invoke(struct _zend_execute_data *execute_data, struct _zval_struct *return_value)
    {
        // check parameter count
        if (!valid(execute_data, return_value)) return;

        // retrieve the parameters
        auto params = parameters(execute_data);

        // catch exceptions thrown by the C++ methods
        try
        {
            // cast the base to the correct object and invoke the member
            (static_cast<T*>(instance(execute_data))->*callback)(params);

            // there is no return value, so we just return null
            yield(return_value, nullptr);
        }
        catch (Throwable &throwable)
        {
            // handle the exception
            handle(throwable);
        }
    }

    /**
     *  Execute the callback
     *
     *  @param  execute_data    data about the PHP call stack
     *  @param  return_value    The value we are returning to PHP
     */
    template <typename T, Value (T::*callback)(Parameters &parameters)>
    static void invoke(struct _zend_execute_data *execute_data, struct _zval_struct *return_value)
    {
        // check parameter count
        if (!valid(execute_data, return_value)) return;

        // retrieve the parameters
        auto params = parameters(execute_data);

        // catch exceptions thrown by the C++ methods
        try
        {
            // cast the base to the correct object and invoke the member
            auto result = (static_cast<T*>(instance(execute_data))->*callback)(params);

            // store the return value in the return_value
            yield(return_value, result);
        }
        catch (Throwable &throwable)
        {
            // handle the exception
            handle(throwable);
        }
    }

    /**
     *  Execute the callback
     *
     *  @param  execute_data    data about the PHP call stack
     *  @param  return_value    The value we are returning to PHP
     */
    template <typename T, Value (T::*callback)(Parameters &parameters) const>
    static void invoke(struct _zend_execute_data *execute_data, struct _zval_struct *return_value)
    {
        // check parameter count
        if (!valid(execute_data, return_value)) return;

        // retrieve the parameters
        auto params = parameters(execute_data);

        // catch exceptions thrown by the C++ methods
        try
        {
            // cast the base to the correct object and invoke the member
            auto result = (static_cast<T*>(instance(execute_data))->*callback)(params);

            // store the return value in the return_value
            yield(return_value, result);
        }
        catch (Throwable &throwable)
        {
            // handle the exception
            handle(throwable);
        }
    }

    /**
     *  Execute the callback
     *
     *  @param  execute_data    data about the PHP call stack
     *  @param  return_value    The value we are returning to PHP
     */
    template <void(*callback)()>
    static void invoke(struct _zend_execute_data *execute_data, struct _zval_struct *return_value)
    {
        // catch exceptions thrown by the C++ methods
        try
        {
            // execute the callback
            callback();

            // there is no return value, so we just return null
            yield(return_value, nullptr);
        }
        catch (Throwable &throwable)
        {
            // handle the exception
            handle(throwable);
        }
    }

    /**
     *  Execute the callback
     *
     *  @param  execute_data    data about the PHP call stack
     *  @param  return_value    The value we are returning to PHP
     */
    template <Value(*callback)()>
    static void invoke(struct _zend_execute_data *execute_data, struct _zval_struct *return_value)
    {
        // catch exceptions thrown by the C++ methods
        try
        {
            // execute the callback
            auto result = callback();

            // store the return value in the return_value
            yield(return_value, result);
        }
        catch (Throwable &throwable)
        {
            // handle the exception
            handle(throwable);
        }
    }

    /**
     *  Execute the callback
     *
     *  @param  execute_data    data about the PHP call stack
     *  @param  return_value    The value we are returning to PHP
     */
    template <void(*callback)(Parameters &parameters)>
    static void invoke(struct _zend_execute_data *execute_data, struct _zval_struct *return_value)
    {
        // check parameter count
        if (!valid(execute_data, return_value)) return;

        // retrieve the parameters
        auto params = parameters(execute_data);

        // catch exceptions thrown by the C++ methods
        try
        {
            // execute the callback
            callback(params);

            // there is no return value, so we just return null
            yield(return_value, nullptr);
        }
        catch (Throwable &throwable)
        {
            // handle the exception
            handle(throwable);
        }
    }

    /**
     *  Execute the callback
     *
     *  @param  execute_data    data about the PHP call stack
     *  @param  return_value    The value we are returning to PHP
     */
    template <Value(*callback)(Parameters &parameters)>
    static void invoke(struct _zend_execute_data *execute_data, struct _zval_struct *return_value)
    {
        // check parameter count
        if (!valid(execute_data, return_value)) return;

        // retrieve the parameters
        auto params = parameters(execute_data);

        // catch exceptions thrown by the C++ methods
        try
        {
            // execute the callback
            auto result = callback(params);

            // store the return value in the return_value
            yield(return_value, result);
        }
        catch (Throwable &throwable)
        {
            // handle the exception
            handle(throwable);
        }
    }
};

/**
 *  End namespace
 */
}
