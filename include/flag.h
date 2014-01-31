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
     *  class FlagConcrete
     *  Designed for the safe transfer of a Zend flag to a Zend functions
     */
    template <class AccT>
    class FlagConcrete: public BaseFlag
    {
    public:
        /**
         *  Constructors
         */
        FlagConcrete(const AccT &zflag);
        FlagConcrete(const FlagConcrete &flags) : BaseFlag(flags) {}
        FlagConcrete(FlagConcrete &&flags) : BaseFlag(flags) {}

        /**
         *  Bitwise OR assignment operator
         */
        FlagConcrete &operator|=(const FlagConcrete &flags) {
            _val |= flags._val;
            return *this;
        }

        /**
         *  Bitwise OR operator
         */
        FlagConcrete operator|(const FlagConcrete &flags) {
            return FlagConcrete (_val | flags._val);
        }

        /**
         *  Destructor
         */
        virtual ~FlagConcrete() {}

    private:
        /**
         *  Constructor
         *  @param  int val
         */
        FlagConcrete(const int &val) : BaseFlag(val) {}
    };
    

    typedef FlagConcrete<Zend::AccClass> FlagClass;
    typedef FlagConcrete<Zend::AccMemb> FlagMemb;
    

    FlagClass Flag(const Zend::AccClass &zflag);
    FlagMemb Flag(const Zend::AccMemb &zflag);


/**
 *  End of namespace Php
 */
}

#endif  /* PHPCPP_FLAG_INCLUDE_C_H_ */