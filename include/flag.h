/**
 *  flag.h
 *
 *  flag namespace
 *
 *  @author Valeriy_Dmitriev <ufabiz@gmail.com>
 */

#ifndef PHPCPP_FLAG_INCLUDE_C_H_
#define PHPCPP_FLAG_INCLUDE_C_H_

/**
 *  Namespace Php
 */
namespace Php {


    /**
     *  abstract class BaseFlag 
     *  Designed for the safe transfer of a Zend flag to a Zend functions
     */
    class BaseFlag
    {
    public:
        /**
         *  Copy constructor
         *  @param  BaseFlag      The BaseFlag to copy
         */
        BaseFlag(const BaseFlag &flags) : _val(flags._val) {}
        
        /**
         *  Move constructor
         *  @param  BaseFlag      The BaseFlag to move
         */
        BaseFlag(BaseFlag &&flags) : _val(std::move(flags._val)){}

        /**
         *  Cast to a int
         *  @return int
         */
        operator int () const {
            return _val;
        }

        /**
         *  Destructor
         */
        virtual ~BaseFlag () {}

    protected:

        /**
         *  Constructor
         *  @param  int val
         */
        BaseFlag(const int &val) :_val(val) {}

        /**
         *  Constructor
         *  @param  void
         */
        BaseFlag() {}

        /**
         *  value of flag
         */
        int _val;
    };
    
    /**
     *  class FlagClass
     *  Designed for the safe transfer of a Zend flag to a Zend functions
     */
    class FlagClass: public BaseFlag
    {
    public:
        /**
         *  Constructors
         */
        FlagClass(const Zend::AccClass &zflag);
        FlagClass(const FlagClass &flags) : BaseFlag(flags) {}
        FlagClass(FlagClass &&flags) : BaseFlag(flags) {}

        /**
         *  Bitwise OR assignment operator
         */
        FlagClass &operator|=(const FlagClass &flags) {
            _val |= flags._val;
            return *this;
        }

        /**
         *  Bitwise OR operator
         */
        FlagClass operator|(const FlagClass &flags) {
            return FlagClass (_val | flags._val);
        }

        /**
         *  Destructor
         */
        virtual ~FlagClass() {}

    private:
        /**
         *  Constructor
         *  @param  int val
         */
        FlagClass(const int &val) : BaseFlag(val) {}
    };
    
    /**
     *  class FlagMemb
     *  Flag access to a class member
     *  Designed for the safe transfer of a Zend flag to a Zend functions
     */
    class FlagMemb: public BaseFlag
    {
    public:
        /**
         *  Constructors
         */
        FlagMemb(const Zend::AccMemb &zflag);
        FlagMemb(const FlagMemb &flags) : BaseFlag(flags) {}
        FlagMemb(FlagMemb &&flags) : BaseFlag(flags) {}

        /**
         *  Bitwise OR assignment operator
         */
        FlagMemb &operator|=(const FlagMemb &flags) {
            _val |= flags._val;
            return *this;
        }

        /**
         *  Bitwise OR operator
         */
        FlagMemb operator|(const FlagMemb &flags) {
            return FlagMemb (_val | flags._val);
        }

        /**
         *  Destructor
         */
        virtual ~FlagMemb() {}

    private:
        /**
         *  Constructor
         *  @param  int val
         */
        FlagMemb(const int &val) : BaseFlag(val) {}
    };


    FlagClass Flag(const Zend::AccClass &zflag);
    FlagMemb Flag(const Zend::AccMemb &zflag);


/**
 *  End of namespace Php
 */
}

#endif  /* PHPCPP_FLAG_INCLUDE_C_H_ */