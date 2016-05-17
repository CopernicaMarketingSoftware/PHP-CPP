/**
 *  Ini.h
 *
 *  Class that can be instantiated in the get_module() startup function to
 *  define settings from the php.ini file that are supported.
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Forward declarations
 */
struct _zend_ini_entry;

/**
 *  Set up namespace
 */
namespace Php {


/**
 *  Class definition
 */
class PHPCPP_EXPORT Ini
{
public:
    /**
     *  Supported place-types for ini setting
     *
     *  The possible settings for where the configuration can be changed are:
     *  PHP_INI_USER, PHP_INI_PERDIR, PHP_INI_SYSTEM and PHP_INI_ALL
     *
     *  Usually you would choose where the setting can be changed based on how
     *  it is used. For example if you want to access the setting during RINIT
     *  stage then you would want PHP_INI_PERDIR because the setting would have
     *  no use after RINIT.
     */
    enum Place : int {
        User      = (1<<0),                         // ZEND_INI_USER   (1<<0)
        Perdir    = (1<<1),                         // ZEND_INI_PERDIR (1<<1)
        System    = (1<<2),                         // ZEND_INI_SYSTEM (1<<2)
        All       = (1<<0) | (1<<1) | (1<<2)        // ZEND_INI_ALL  (ZEND_INI_USER|ZEND_INI_PERDIR|ZEND_INI_SYSTEM)
    };

    /**
     *  Constructors for string values
     *
     *  @param  name        Name of the php.ini variable
     *  @param  value       Default value
     *  @param  place       Place where the ini setting can be changed
     */
    Ini(const char *name, const char *value, const Place place = Place::All) :
        _name(name), _value(value), _place(place) {}

    /**
     *  Constructors for bool values
     *
     *  @param  name        Name of the php.ini variable
     *  @param  value       Default value
     *  @param  place       Place where the ini setting can be changed
     */
    Ini(const char *name, bool value, const Place place = Place::All) :
        _name(name), _value(bool2str(value)), _place(place) {}

    /**
     *  Constructors for integer values
     *
     *  @param  name        Name of the php.ini variable
     *  @param  value       Default value
     *  @param  place       Place where the ini setting can be changed
     */
    Ini(const char *name, const int16_t value, const Place place = Place::All) :
        _name(name), _value(std::to_string(value)), _place(place) {}

    Ini(const char *name, const int32_t value, const Place place = Place::All) :
        _name(name), _value(std::to_string(value)), _place(place) {}

    Ini(const char *name, const int64_t value, const Place place = Place::All) :
        _name(name), _value(std::to_string(value)), _place(place) {}

    /**
     *  Constructors for floating point values
     *
     *  @param  name        Name of the php.ini variable
     *  @param  value       Default value
     *  @param  place       Place where the ini setting can be changed
     */
    Ini(const char *name, const double value, const Place place = Place::All) :
        _name(name), _value(std::to_string(value)), _place(place) {}


    /**
     *  Filling ini_entries
     *  @param  ini_entry
     *  @param  module_number
     */
    void fill(struct _zend_ini_entry_def *ini_entry, int module_number);


private:
    /**
     *  Helper function to convert a boolean to a string
     *  @param  value
     *  @return string
     */
#ifdef _MSC_VER
    static const char* bool2str(const bool value)
#else
    static constexpr const char* bool2str(const bool value)
#endif
    {
        // cast to a string
        return (  static_cast<bool>(value) ? "On" : "Off");
    }

    /**
     *  ini entry name
     *  @var    std::string
     */
    std::string _name;

    /**
     *  ini entry value
     *  @var    std::string
     */
    std::string _value;

    /**
     *  Place where the configuration can be changed
     *  @var    Place
     */
    Place _place;


};

/**
 *  End of namespace
 */
}
