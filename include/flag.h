/**
 *  flag.h
 *
 *  flag clases for the safe transfer of a Zend flag to a Zend functions
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
     *  class FlagTemplate 
     *  Designed for the safe transfer of a Zend flag to a Zend functions
     */
    template <class AccT>
    class FlagTemplate
    {
    public:
        /**
         *  Constructor
         */
        FlagTemplate(const AccT &zflag);

        /**
         *  Copy constructor
         *  @param  FlagTemplate      The FlagTemplate to copy
         */
        FlagTemplate(const FlagTemplate &flags) : _val(flags._val) {}
        
        /**
         *  Move constructor
         *  @param  FlagTemplate      The FlagTemplate to move
         */
        FlagTemplate(FlagTemplate &&flags) : _val(std::move(flags._val)){}

        /**
         *  Assignment operator
         */
        FlagTemplate &operator=(const FlagTemplate &flags) {
            if (this != &flags) {
                _val = flags._val;
            }
            return *this;
        }

        /**
         *  Move operator
         */
        FlagTemplate &operator=(FlagTemplate &&flags) {
            if (this != &flags) {
                _val = std::move(flags._val);
            }
            return *this;
        }

        /**
         *  Bitwise OR assignment operator
         */
        FlagTemplate &operator|=(const FlagTemplate &flags) {
            _val |= flags._val;
            return *this;
        }

        /**
         *  Bitwise OR operator
         */
        FlagTemplate operator|(const FlagTemplate &flags) {
            return FlagTemplate (_val | flags._val);
        }

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
        ~FlagTemplate () {}

    private:

        /**
         *  Private constructor
         *  @param  int val
         */
        FlagTemplate(const int &val) :_val(val) {}

        /**
         *  Private constructor
         *  @param  void
         */
        FlagTemplate() {}

        /**
         *  value of flag
         */
        int _val;
    };
    
    /**
     *  class FlagClass 
     *  For the safe transfer of a Zend Class flags to a Zend functions
     */
    typedef FlagTemplate<Zend::AccClass> FlagClass;
    /**
     *  class FlagClass 
     *  For the safe transfer of a Zend access types for methods and propertyes
     */
    typedef FlagTemplate<Zend::AccMemb> FlagMemb;
    

    /**
     *  factory function
     */
    FlagClass Flag(const Zend::AccClass &zflag);
    FlagMemb  Flag(const Zend::AccMemb &zflag);


/**
 *  End of namespace Php
 */
}

#endif  /* PHPCPP_FLAG_INCLUDE_C_H_ */