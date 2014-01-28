/**
 *  zend.cpp
 *
 *  zend namespace
 *
 *  @author Valeriy_Dmitriev <ufabiz@gmail.com>
 */

#include "includes.h"
#include "../include/zend.h"


/**
 *  Namespace Php
 */
namespace Php {

    /**
     *  collection of Zend constants
     */
    namespace Zend {
        
        /**
         *  Constructor
         *  @param  prop        instance of FlagClass
         */
        Flag::Flag(const FlagClass &prop) {
            /**
             *  access types for methods
             *  (method flags)
             */
            switch(prop){
                    // ZEND_ACC_IMPLICIT_ABSTRACT_CLASS is used for abstract classes (since it is set by any abstract method even interfaces MAY have it set, too).
                    case FlagClass::IMPLICIT_ABSTRACT:
                        _val = ZEND_ACC_IMPLICIT_ABSTRACT_CLASS;     //0x10
                    break;
                    //ZEND_ACC_EXPLICIT_ABSTRACT_CLASS denotes that a class was explicitly defined as abstract by using the keyword.
                    case FlagClass::ABSTRACT:
                        _val = ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;     //0x20;
                    break;
                    case FlagClass::FINAL:
                        _val = ZEND_ACC_FINAL_CLASS;                 //0x40;
                    break;
                    case FlagClass::INTERFACE:
                        _val = ZEND_ACC_INTERFACE;                   //0x80;
                    break;
                    case FlagClass::TRAIT:
                        _val = ZEND_ACC_TRAIT;                       //0x120;
                    break;
                    default:
                        _val = 0;
            }
        }

        /**
         *  Constructor
         *  @param  prop        instance of FlagMethod
         */
        Flag::Flag(const FlagMethod &prop){
            /**
             *  access types for classes
             *  (class flags)
             */
            switch(prop){
                    case FlagMethod::STATIC:
                        _val = ZEND_ACC_STATIC;                    //0x01
                    break;
                    case FlagMethod::ABSTRACT:
                        _val = ZEND_ACC_ABSTRACT;                  //0x02
                    break;
                    case FlagMethod::FINAL:
                        _val = ZEND_ACC_FINAL;                     //0x04
                    break;
                    case FlagMethod::IMPLEMENTED_ABSTRACT:
                        _val = ZEND_ACC_IMPLEMENTED_ABSTRACT;      //0x08
                    break;
                    //method flag (bc only), any method that has this flag can be used statically and non statically.
                    case FlagMethod::ALLOW_STATIC:
                        _val = ZEND_ACC_ALLOW_STATIC;              //0x10000                
                    break;

                    // method flags (visibility)
                    // The order of those must be kept - public < protected < private
                    case FlagMethod::PUBLIC:
                        _val = ZEND_ACC_PUBLIC;             //0x100
                    break;
                    case FlagMethod::PROTECTED:
                        _val = ZEND_ACC_PROTECTED;                 //0x200
                    break;
                    case FlagMethod::PRIVATE:
                        _val = ZEND_ACC_PRIVATE;                   //0x400
                    break;
                    case FlagMethod::PPP_MASK:
                        _val = ZEND_ACC_PPP_MASK;                  //(ZEND_ACC_PUBLIC | ZEND_ACC_PROTECTED | ZEND_ACC_PRIVATE)                
                    break;

                   //shadow of parent's private method/property
                    case FlagMethod::SHADOW:
                        _val = ZEND_ACC_SHADOW;                       //0x20000
                    break;
                    default:
                        _val = 0;
            }
        }

        /**
         *  Copy constructor
         *  @param  Flag      The Flag to copy
         */
        Flag::Flag(const Flag &prop) {
            _val = prop._val;
        }
            
        /**
         *  Move constructor
         *  @param  Flag      The Flag to move
         */
        Flag::Flag(Flag &&prop) {
            _val = prop._val;
            //_val = std::move(prop._val);
        }


    }
    /**
     *  End of namespace Zend
     */



/**
 *  End of namespace Php
 */
}
