/**
 *  Macros.h
 * 
 *  Some macros have changed or have been removed between PHP versions.
 *  In this file we write custom versions of such macros to overcome that
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2023 Copernica BV
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  The "SEPARATE_ZVAL_IF_NOT_REF()" macro has been removed
 */
#ifndef SEPARATE_ZVAL_IF_NOT_REF
#define SEPARATE_ZVAL_IF_NOT_REF(zv) do {           \
            zval *_zv = (zv);                   \
            if (Z_TYPE_P(_zv) == IS_ARRAY) {    \
                    SEPARATE_ARRAY(_zv);        \
            }                                   \
        } while(0)
#endif
