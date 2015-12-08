/**
 *  IniValue.h
 *
 *  Class IniValue designed for extracting values from ini entries
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class IniValue designed for extracting values from ini entries.
 */
class PHPCPP_EXPORT IniValue
{
public:
    /**
     *  Constructors
     *
     *  @param  name        Name of the php.ini variable
     *  @param  isorig      Is the original value
     */
    IniValue(const char *name, const bool isorig) : _name(name), _isorig(isorig) {}

    /**
     *  Cast to a 16-bit number
     *  @return int16_t
     */
    operator int16_t () const
    {
        return static_cast<int16_t>(numericValue());
    }

    /**
     *  Cast to a 32-bit number
     *  @return int32_t
     */
    operator int32_t () const
    {
        return static_cast<int32_t>(numericValue());
    }

    /**
     *  Cast to a 64-bit number
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
        return boolValue();
    }

    /**
     *  Cast to a string
     *  @return string
     */
    operator std::string () const
    {
        return stringValue();
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
     *  Boolean value
     *  @return bool
     */
    bool boolValue() const
    {
        return static_cast<bool>(numericValue());
    }

    /**
     *  String value
     *  @return std::string
     */
    std::string stringValue() const
    {
        auto value = rawValue();
        return std::string(value ? value : "");
    }

    /**
     *  Get access to the raw buffer for read operations.
     *  @return const char *
     */
    const char *rawValue() const;


private:


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
PHPCPP_EXPORT std::ostream &operator<<(std::ostream &stream, const IniValue &ini_val);


/**
 *  End of namespace
 */
}
