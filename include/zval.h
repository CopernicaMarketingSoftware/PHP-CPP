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
 *  Dependencies
 */
#include <type_traits>

/**
 *  Forward declarations
 */
struct _zval_struct;

/**
 *  Typedef for zval aligned storage
 */
using aligned_zval_struct = typename std::aligned_storage<16>::type;

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
     *  @var    aligned_zval_struct
     */
    aligned_zval_struct _buffer;

public:
    /**
     *  Cast to a zval
     *
     *  @return zend value structure
     */
    operator _zval_struct * () const &
    {
        // reinterpret the value as a zval
        return const_cast<struct _zval_struct*>(reinterpret_cast<const struct _zval_struct*>(&_buffer));
    }

    /**
     *  Cast to a zval
     *
     *  @return zend value structure
     */
    struct _zval_struct &operator* () const &
    {
        // reinterpret and dereference
        return *const_cast<struct _zval_struct*>(reinterpret_cast<const struct _zval_struct*>(&_buffer));
    }

    /**
     *  In case this is a reference, dereference it into a zval.. In case it isn't
     *  the actual value is returned.
     *  @return struct _zval_struct*
     */
    struct _zval_struct *dereference() const;
};

/**
 *  End namespace
 */
}
