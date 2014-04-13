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
 *  Class IniValue designed for extracting values from ini entries.
 */
class IniValue
{
public:
    /**
     *  Constructors for floating point values
     * 
     *  @param  name        Name of the php.ini variable
     *  @param  isorig      Is the original value
     */
    IniValue(const char *name, const bool isorig) : _name(name), _isorig(isorig) {}

    /**
     *  Cast to a number
     *  @return int32_t
     */
    operator int16_t () const
    {
        return (int16_t)numericValue();
    }

    /**
     *  Cast to a number
     *  @return int32_t
     */
    operator int32_t () const
    {
        return (int32_t)numericValue();
    }

    /**
     *  Cast to a number
     *  @return uint64_t
     */
    operator int64_t () const
    {
        return numericValue();
    }
    
    /**
     *  Cast to a boolean
     *  @return boolean
     */
    operator bool () const
    {
        return (bool)numericValue();
    }
    
    /**
     *  Cast to a string
     *  @return string
     */
    operator std::string () const
    {
        return rawValue();
    }
    
    /**
     *  Cast to byte array
     *  @return const char *
     */
    operator const char * () const
    {
        return rawValue();
    }
    
    /**
     *  Cast to a floating point
     *  @return double
     */
    operator double() const;


private:

    
    /**
     *  Retrieve the value as number
     *
     *  We force this to be a int64_t because we assume that most
     *  servers run 64 bits nowadays, and because we use int32_t, int64_t
     *  almost everywhere, instead of 'long' and on OSX neither of
     *  these intxx_t types is defined as 'long'...
     *
     *  @return int64_t
     */
    int64_t numericValue() const;

    /**
     *  Get access to the raw buffer for read operationrs.
     *  @return const char *
     */
    const char *rawValue() const;
    
    /**
     *  ini entry name
     *  @var    std::string
     */
    std::string _name;

    /**
     *  Is the orig value?
     *  @var    bool
     */
    bool _isorig = false;
};

/**
 *  Custom output stream operator
 *  @param  stream
 *  @param  ini_val
 *  @return ostream
 */
std::ostream &operator<<(std::ostream &stream, const IniValue &ini_val)
{
    return stream << static_cast<const char*>(ini_val);
}


/**
 *  Class definition
 */
class Ini
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
     *  @param  orig        Original value (if the user resets the variable, it is set back to this value)
     *  @param  place       Place where the ini setting can be changed
     */
    Ini(const char *name, const char *value, const char *orig, const Place place = Place::All) :
        _name(name), _value(value), _orig(orig), _place(place) {}

    Ini(const char *name, const char *value, const Place place = Place::All) :
        _name(name), _value(value), _orig_empty(true), _place(place) {}

    /**
     *  Constructors for bool values
     * 
     *  @param  name        Name of the php.ini variable
     *  @param  value       Default value
     *  @param  orig        Original value (if the user resets the variable, it is set back to this value)
     *  @param  place       Place where the ini setting can be changed
     */
    Ini(const char *name,const  bool value, const bool orig, const Place place = Place::All) :
        _name(name), _value(bool2str(value)), _orig(bool2str(orig)), _place(place) {}

    Ini(const char *name, const bool value, const Place place = Place::All) :
        _name(name), _value(bool2str(value)), _orig_empty(true), _place(place) {}

    /**
     *  Constructors for integer values
     * 
     *  @param  name        Name of the php.ini variable
     *  @param  value       Default value
     *  @param  orig        Original value (if the user resets the variable, it is set back to this value)
     *  @param  place       Place where the ini setting can be changed
     */
    Ini(const char *name, const int16_t value, const int16_t orig, const Place place = Place::All) :
        _name(name), _value(std::to_string(value)), _orig(std::to_string(orig)), _place(place) {}

    Ini(const char *name, const int16_t value, const Place place = Place::All) :
        _name(name), _value(std::to_string(value)), _orig_empty(true), _place(place) {}

    Ini(const char *name, const int32_t value, const int32_t orig, const Place place = Place::All) :
        _name(name), _value(std::to_string(value)), _orig(std::to_string(orig)), _place(place) {}

    Ini(const char *name, const int32_t value, const Place place = Place::All) :
        _name(name), _value(std::to_string(value)), _orig_empty(true), _place(place) {}

    Ini(const char *name, const int64_t value, const int64_t orig, const Place place = Place::All) :
        _name(name), _value(std::to_string(value)), _orig(std::to_string(orig)), _place(place) {}

    Ini(const char *name, const int64_t value, const Place place = Place::All) :
        _name(name), _value(std::to_string(value)), _orig_empty(true), _place(place) {}

    /**
     *  Constructors for floating point values
     * 
     *  @param  name        Name of the php.ini variable
     *  @param  value       Default value
     *  @param  orig        Original value (if the user resets the variable, it is set back to this value)
     *  @param  place       Place where the ini setting can be changed
     */
    Ini(const char *name, const double value, const double orig, const Place place = Place::All) :
        _name(name), _value(std::to_string(value)), _orig(std::to_string(orig)), _place(place) {}

    Ini(const char *name, const double value, const Place place = Place::All) :
        _name(name), _value(std::to_string(value)), _orig_empty(true), _place(place) {}


    /**
     *  Filling ini_entries
     *  @param  ini_entry
     *  @param  module_number
     */
    void fill(struct _zend_ini_entry *ini_entry, int module_number);

    
    static IniValue get(const char* name)
    {
        return IniValue(name, false);
    }

    static IniValue get_orig(const char* name)
    {
        return IniValue(name, true);
    }


private:
    /**
     *  Helper function to convert a boolean to a string
     *  @param  value
     *  @return string
     */
    static constexpr const char* bool2str(const bool value)
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
     *  ini entry original value
     *  @var    std::string
     */
    std::string _orig;

    /**
     *  Is the orig value set or empty?
     *  @var    bool
     */
    bool _orig_empty = false;
    
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

