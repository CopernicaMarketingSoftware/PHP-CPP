/**
 *  HiddenPointer.h
 *
 *  Helper class that we use to hide a pointer in a string. We do this
 *  by creating a string buffer that is a littlebit bigger, and put
 *  the hidden pointer in front of the name
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
template <typename Type>
class HiddenPointer
{
public:
    /**
     *  Constructor to hide the pointer in a buffer
     *  @param  pointer     The pointer to hide
     *  @param  text        The visible text
     *  @param  size        Optional text size
     */
    HiddenPointer(Type *pointer, const char *text, int size=-1)
    {
        // calculate size of the text
        if (size < 0) size = ::strlen(text);

        // allocate data + trailing null + size of pointer
        char *buffer = new char[size + 1 + sizeof(Type *)];

        // copy pointer into the buffer
        memcpy(buffer, &pointer, sizeof(Type *));
        
        // copy the name into the buffer
        memcpy(buffer + sizeof(Type *), text, size + 1);
        
        // store in member
        _buffer = buffer;
    }
    
    /**
     *  Hide pointer in buffer
     *  @param  pointer
     *  @param  text
     */
    HiddenPointer(Type *pointer, const std::string &text) : 
        HiddenPointer(pointer, text.c_str(), text.size()) {}

    /**
     *  Constructor to retrieve the object given a buffer
     *  @param  text        The visible text
     */
    HiddenPointer(const char *text) : 
        _buffer(text - sizeof(Type *)), // the buffer starts before the actual text
        _allocated(false) {}            // no memory was allocated
    
    /**
     *  Copy constructor
     *  @param  that
     */
    HiddenPointer(const HiddenPointer<Type> &that) : _allocated(that._allocated)
    {
        // is the other object allocated?
        if (_allocated)
        {
            // allocate this object too, call other constructor
            HiddenPointer(that, that);
        }
        else
        {
            // just copy the data
            _buffer = that._buffer;
        }
    }
    
    /**
     *  Move constructor
     *  @param  that
     */
    HiddenPointer(HiddenPointer<Type> &&that) _NOEXCEPT : _allocated(that._allocated), _buffer(that._buffer)
    {
        // the other object is no longer allocated
        that._allocated = false;
    }
    
    /**
     *  Destructor
     */
    virtual ~HiddenPointer() 
    {
        // destruct data
        if (_allocated) delete[] _buffer;
    }
    
    /**
     *  Assignment operator
     *  @param  that
     *  @return HiddenPointer
     */
    HiddenPointer<Type> &operator=(const HiddenPointer &that)
    {
        // skip self assignmend
        if (this == &that) return *this;
        
        // deallocate current object
        if (_allocated) delete[] _buffer;
        
        // copy allocated setting
        _allocated = that._allocated;
        
        // is the other object allocated?
        if (_allocated)
        {
            // allocate this object too, call constructor
            HiddenPointer(that, that);
        }
        else
        {
            // just copy the data
            _buffer = that._buffer;
        }
        
        // done
        return *this;
    }
    
    /**
     *  Retrieve the pointer
     *  @return Type*
     */
    operator Type * () const
    {
        // type is stored in front of the buffer
        return *((Type **)_buffer);
    }
    
    /**
     *  Retrieve the text
     *  @return const char *
     */
    operator const char * () const
    {
        // name starts a number of bytes further
        return _buffer + sizeof(Type *);
    }
    
    /**
     *  Derefence the pointer
     *  @return Type*
     */
    Type *operator->() const
    {
        // type is stored in front of the buffer
        return *((Type **)_buffer);
    }
    
private:
    /**
     *  Buffer that holds both the pointer and the text - the text starts
     *  a number of bytes further up
     *  @var buffer
     */
    const char *_buffer = nullptr;

    /**
     *  Was this allocated?
     *  @var bool
     */
    bool _allocated = true;
};

/**
 *  End of namespace
 */
}

