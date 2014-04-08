/**
 *  Ini.h
 *
 *  
 *  
 *
 *  @copyright 2013 Copernica BV
 */

/**
 *  Forward declaration
 */

struct _zend_ini_entry;

/**
 *  Set up namespace
 */
namespace Php {


    /**
     *  Class definition
     */
    class Ini
    {
    public:

        /**
         *  Supported place-types for ini setting
         *  The possible settings for where the configuration can be changed are:
         *  PHP_INI_USER, PHP_INI_PERDIR, PHP_INI_SYSTEM and PHP_INI_ALL
         *  Usually you would choose where the setting can be changed based on how it is used. For example if you want to access
         *  the setting during RINIT stage then you would want PHP_INI_PERDIR because the setting would have no use after RINIT.
         */
        enum class Place : int {
            User      = (1<<0),                         // ZEND_INI_USER   (1<<0)
            Perdir    = (1<<1),                         // ZEND_INI_PERDIR (1<<1)
            System    = (1<<2),                         // ZEND_INI_SYSTEM (1<<2)
            All       = (1<<0) | (1<<1) | (1<<2)        // ZEND_INI_ALL  (ZEND_INI_USER|ZEND_INI_PERDIR|ZEND_INI_SYSTEM)
        };


        /**
         *  default constructors
         */
        Ini(const char *name, const char *value, const char *orig_value, const Place place = Place::All) :
            _name(name), _value(value), _orig_value(orig_value), _place(place) {}

        Ini(const char *name, const char *value, const Place place = Place::All) :
            _name(name), _value(value), _orig_empty(true), _place(place) {}

        /**
         *  Constructors for bool value
         */
        Ini(const char *name,const  bool value, const bool orig_value, const Place place = Place::All) :
            _name(name), _value(bool2str(value)), _orig_value(bool2str(orig_value)), _place(place) {}

        Ini(const char *name, const bool value, const Place place = Place::All) :
            _name(name), _value(bool2str(value)), _orig_empty(true), _place(place) {}

         /**
         *  Constructors for integer value
         *  @param  value
          */
        Ini(const char *name, const int16_t value, const int16_t orig_value, const Place place = Place::All) :
            _name(name), _value(std::to_string(value)), _orig_value(std::to_string(orig_value)), _place(place) {}

        Ini(const char *name, const int16_t value, const Place place = Place::All) :
            _name(name), _value(std::to_string(value)), _orig_empty(true), _place(place) {}


        Ini(const char *name, const int32_t value, const int32_t orig_value, const Place place = Place::All) :
            _name(name), _value(std::to_string(value)), _orig_value(std::to_string(orig_value)), _place(place) {}

        Ini(const char *name, const int32_t value, const Place place = Place::All) :
            _name(name), _value(std::to_string(value)), _orig_empty(true), _place(place) {}


        Ini(const char *name, const int64_t value, const int64_t orig_value, const Place place = Place::All) :
            _name(name), _value(std::to_string(value)), _orig_value(std::to_string(orig_value)), _place(place) {}

        Ini(const char *name, const int64_t value, const Place place = Place::All) :
            _name(name), _value(std::to_string(value)), _orig_empty(true), _place(place) {}

         /**
         *  Constructors for float value
         *  @param  value
          */
        Ini(const char *name, const double value, const double orig_value, const Place place = Place::All) :
            _name(name), _value(std::to_string(value)), _orig_value(std::to_string(orig_value)), _place(place) {}

        Ini(const char *name, const double value, const Place place = Place::All) :
            _name(name), _value(std::to_string(value)), _orig_empty(true), _place(place) {}


        /**
         *  Copy constructor
         *  @param  Ini
         */
        Ini(const Ini &that) :
            _name(that._name), _value(that._value), _orig_value(that._orig_value), _orig_empty(that._orig_empty), _place(that._place) 
        {}

        /**
         *  Move constructor
         *  @param  Ini
         */
        Ini(Ini &&that) :
            _name(that._name), _value(std::move(that._value)), _orig_value(std::move(that._orig_value)), _orig_empty(that._orig_empty), _place(that._place)
        {}


        /**
         *  Filling ini_entries
         *  @param  zend_ini_entry *ini_entry, int module_number
         *  @param  int module_number
         */
        void fill(_zend_ini_entry *ini_entry, int module_number);
        



    private:

        static constexpr const char* bool2str(const bool value)
        {
            return (  static_cast<bool>(value) ? "On" : "Off");
        }
        
        // ini entry name
        const char* _name;

        // ini entry value
        std::string _value;

        // ini entry original value
        std::string _orig_value;

        // _orig_value is empty
        bool _orig_empty = false;
        
        // plase where the configuration can be changed
        Place _place;
    };



/**
 *  End of namespace
 */
}

