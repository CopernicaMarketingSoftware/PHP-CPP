/**
 *  zend.h
 *
 *  zend namespace
 *
 *  @author Valeriy_Dmitriev <ufabiz@gmail.com>
 */

#ifndef PHPCPP_ZEND_INCLUDE_C_H_
#define PHPCPP_ZEND_INCLUDE_C_H_

/**
 *  Namespace Php
 */
namespace Php {

    /**
     *  collection of Zend constants
     */
    namespace Zend {

        /**
         *  access types (flags)
         *  see Zend/zend_compile.h
         */
        
        /**
         *  access types for methods and properties
         *  (method flags)
         */
        enum class AccMemb {
            //  method flags (types):
            STATIC                  ,//= ZEND_ACC_STATIC,                    //0x01
            ABSTRACT                ,//= ZEND_ACC_ABSTRACT,                  //0x02
            FINAL                   ,//= ZEND_ACC_FINAL,                     //0x04
            //IMPLEMENTED_ABSTRACT    ,//= ZEND_ACC_IMPLEMENTED_ABSTRACT,      //0x08
            //method flag (bc only), any method that has this flag can be used statically and non statically.
            //ALLOW_STATIC            ,//= ZEND_ACC_ALLOW_STATIC,              //0x10000                

            // method flags (visibility)
            // The order of those must be kept - public < protected < private
            PUBLIC                  ,//= ZEND_ACC_PUBLIC,             //0x100
            PROTECTED               ,//= ZEND_ACC_PROTECTED,                 //0x200
            PRIVATE                 ,//= ZEND_ACC_PRIVATE,                   //0x400
            PPP_MASK                ,//= ZEND_ACC_PPP_MASK,                  //(ZEND_ACC_PUBLIC | ZEND_ACC_PROTECTED | ZEND_ACC_PRIVATE)                

            // method flags (special method detection)
            //CTOR                    ,//= ZEND_ACC_CTOR,                      //0x2000
            //DTOR                    ,//= ZEND_ACC_DTOR,                      //0x4000
            //CLONE                   ,//= ZEND_ACC_CLONE,                     //0x8000

            //shadow of parent's private method/property
            //SHADOW                  ,//= ZEND_ACC_SHADOW                       //0x20000

            // additional field for designation flag CONSTANT. Not in the Zend engine
            CONSTANT                ,//= 0                                     //0
        };

        /**
         *  access types for classes
         *  (class flags)
         */
        enum class AccClass {
            NOSET                   ,//=      ..       ,                       //0
            // ZEND_ACC_IMPLICIT_ABSTRACT_CLASS is used for abstract classes (since it is set by any abstract method even interfaces MAY have it set, too).
            //IMPLICIT_ABSTRACT       ,//= ZEND_ACC_IMPLICIT_ABSTRACT_CLASS,     //0x10
            //ZEND_ACC_EXPLICIT_ABSTRACT_CLASS denotes that a class was explicitly defined as abstract by using the keyword.
            ABSTRACT                ,//= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS,     //0x20
            FINAL                   ,//= ZEND_ACC_FINAL_CLASS,                 //0x40
            INTERFACE               ,//= ZEND_ACC_INTERFACE,                   //0x80

            //class implement interface(s) flag,
            //IMPLEMENT_INTERFACES    ,//= ZEND_ACC_IMPLEMENT_INTERFACES,         //0x80000
            //IMPLEMENT_TRAITS        ,//= ZEND_ACC_IMPLEMENT_TRAITS,             //0x400000
        };


    }
/**
 *  End of namespace Php
 */
}

#endif  /* PHPCPP_ZEND_INCLUDE_C_H_ */
