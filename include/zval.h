/**
 *  zval.h
 *
 *  A buffer to have a zval on the stack without
 *  needing to do any heap allocations
 *
 *  @copyright 2016 Copernica B.V.
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  Forward declarations
 */
struct _zval_struct;

/**
 *  Start namespace
 */
namespace Php
{

/**
 *  The zend value wrapper
 */
class Zval 
{
private:
    /**
     *  The zval buffer
     *  @var    char[]
     */
    char _buffer[16];
public:
    /**
     *  Cast to a zval
     *
     *  @return zend value structure
     */
    operator _zval_struct * () const &
    {
        // reinterpret the value as a zval
        return const_cast<struct _zval_struct*>(reinterpret_cast<const struct _zval_struct*>(_buffer));
    }

    /**
     *  Cast to a zval
     *
     *  @return zend value structure
     */
    struct _zval_struct &operator* () const &
    {
        // reinterpret and dereference
        return *const_cast<struct _zval_struct*>(reinterpret_cast<const struct _zval_struct*>(_buffer));
    }
};

/**
 *  End namespace
 */
}
