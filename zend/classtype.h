/**
 *  @file classtype.h
 *
 *  Internal class types enumeration.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 - 2022 Copernica BV
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Enumeration definition.
 *  This is different for different PHP versions
 */
enum class ClassType {
    Regular     =   0x00,
    Interface   =   ZEND_ACC_INTERFACE,
    Abstract    =   ZEND_ACC_ABSTRACT,
    Final       =   ZEND_ACC_FINAL,
};

/**
 *  End namespace
 */
}
