/**
 *  HardCoded.h
 *
 *  Small class that can be wrapped around a "hardcoded string". Normally, the 
 *  Value object always makes a full copy of a string, because the value object
 *  may exist for a longer period than the pointer-to-a-string that is wrapped
 *  in it.
 *
 *  However, in some situations it is already certain that the buffer in
 *  which the original string is stored will outlive the Value object. This is
 *  for example true for hardcoded strings. Such const-char* can be wrapped into
 *  a Php::HardCoded instance before they are assigned to a Php::Value object
 *
 *  This class is called HardCoded because it is often used for hardcoded 
 *  strings, but you can use it for other values as well.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Php namespace
 */
namespace Php {

/**
 *  Class definition
 */
class HardCoded
{
public:
    /**
     *  Constructors
     *
     *  The class has a regular constructor with a size or not, a copy constructor
     *  and move constructor.
     *
     *  @param  buffer
     *  @param  size
     *  @param  that
     */
    HardCoded(const char *buffer, size_t size) : _buffer(buffer), _size(size) {}
    HardCoded(const char *buffer) : _buffer(buffer), _size(strlen(buffer)) {}
    HardCoded(const char buffer) : _buffer(&buffer), _size(1) {}
    HardCoded(const HardCoded &that) : _buffer(that._buffer), _size(that._size) {}
    HardCoded(HardCoded &&that) : _buffer(that._buffer), _size(that._size) {}

    /**
     *  Destructor
     */
    virtual ~HardCoded() {}
    
    /**
     *  Method to get access to the buffer
     *  @return const char *
     */
    const char *buffer() const
    {
        return _buffer;
    }
    
    /**
     *  Size of the buffer
     *  @return size_t
     */
    size_t size() const
    {
        return _size;
    }

private:
    /**
     *  The actual buffer
     *  @var    const char *
     */
    const char *_buffer;
    
    /**
     *  Size of the buffer
     *  @var    size_t
     */
    size_t _size;
};

/**
 *  End namespace
 */
}

