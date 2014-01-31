/**
 *  flag.cpp
 *
 *  flag namespace
 *
 *  @author Valeriy_Dmitriev <ufabiz@gmail.com>
 */

//#include <typeinfo>
#include "includes.h"


/**
 *  Namespace Php
 */
namespace Php {

    
    /**
     *  Constructor
     *  @param  flags        instance of Zend::AccClass
     */
    template <>
    //FlagClass::FlagClass(const Zend::AccClass &zflag) {
    FlagConcrete<Zend::AccClass>::FlagConcrete(const Zend::AccClass &zflag) {
        /**
         *  access types for methods
         *  (method flags)
         */
        switch(zflag){
                // ZEND_ACC_IMPLICIT_ABSTRACT_CLASS is used for abstract classes (since it is set by any abstract method even interfaces MAY have it set, too).
                case Zend::AccClass::IMPLICIT_ABSTRACT:
                    _val = ZEND_ACC_IMPLICIT_ABSTRACT_CLASS;     //0x10
                break;
                //ZEND_ACC_EXPLICIT_ABSTRACT_CLASS denotes that a class was explicitly defined as abstract by using the keyword.
                case Zend::AccClass::ABSTRACT:
                    _val = ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;     //0x20;
                break;
                case Zend::AccClass::FINAL:
                    _val = ZEND_ACC_FINAL_CLASS;                 //0x40;
                break;
                case Zend::AccClass::INTERFACE:
                    _val = ZEND_ACC_INTERFACE;                   //0x80;
                break;
                case Zend::AccClass::TRAIT:
                    _val = ZEND_ACC_TRAIT;                       //0x120;
                break;
                default:
                    _val = 0;
        }
    }

    /**
     *  Constructor
     *  @param  flags        instance of Zend::AccMemb
     */
    template <>
    //FlagMemb::FlagMemb(const Zend::AccMemb &zflag){
    FlagConcrete<Zend::AccMemb>::FlagConcrete(const Zend::AccMemb &zflag){
        /**
         *  access types for classes
         *  (class flags)
         */
        switch(zflag){
                case Zend::AccMemb::STATIC:
                    _val = ZEND_ACC_STATIC;                    //0x01
                    //_val = ZEND_ACC_ALLOW_STATIC | ZEND_ACC_PUBLIC;
                    //_val = ZEND_ACC_STATIC | ZEND_ACC_PUBLIC;
                break;
                case Zend::AccMemb::ABSTRACT:
                    _val = ZEND_ACC_ABSTRACT;                  //0x02
                break;
                case Zend::AccMemb::FINAL:
                    _val = ZEND_ACC_FINAL;                     //0x04
                break;
                case Zend::AccMemb::IMPLEMENTED_ABSTRACT:
                    _val = ZEND_ACC_IMPLEMENTED_ABSTRACT;      //0x08
                break;

                // method flags (visibility)
                // The order of those must be kept - public < protected < private
                case Zend::AccMemb::PUBLIC:
                    _val = ZEND_ACC_PUBLIC;             //0x100
                break;
                case Zend::AccMemb::PROTECTED:
                    _val = ZEND_ACC_PROTECTED;                 //0x200
                break;
                case Zend::AccMemb::PRIVATE:
                    _val = ZEND_ACC_PRIVATE;                   //0x400
                break;
                case Zend::AccMemb::PPP_MASK:
                    _val = ZEND_ACC_PPP_MASK;                  //(ZEND_ACC_PUBLIC | ZEND_ACC_PROTECTED | ZEND_ACC_PRIVATE)                
                break;

               //shadow of parent's private method/property
                case Zend::AccMemb::SHADOW:
                    _val = ZEND_ACC_SHADOW;                       //0x20000
                break;
                default:
                    _val = 0;
        }
    }


    /**
     *  factory function
     */
    FlagClass Flag(const Zend::AccClass &zflag) {
        return FlagClass(zflag);
    }
    FlagMemb Flag(const Zend::AccMemb &zflag) {
        return FlagMemb(zflag);
    }

/**
 *  End of namespace Php
 */
}
