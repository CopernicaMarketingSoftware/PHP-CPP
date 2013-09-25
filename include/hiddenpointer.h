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
        // calculate size
        if (size < 0) size = strlen(text);

        // reserve enough room for the text and the pointer
        _data.reserve(size + sizeof(Type *));
        
        // store the pointer
        _data.assign(std::string((const char *)&pointer, sizeof(Type *)));
        
        // append the text
        _data.append(text, size);
        
        // store pointers
        _pointer = pointer;
        _text = _data.c_str() + sizeof(Type *);
    }
    
    /**
     *  Hide pointer in buffer
     *  @param  pointer
     *  @param  text
     */
    HiddenPointer(Type *pointer, const std::string &text) : HiddenPointer(pointer, text.c_str(), text.size()) {}

    /**
     *  Constructor to retrieve the object given a buffer
     *  @param  text        The visible text
     *  @param  size        Size of the text
     */
    HiddenPointer(const char *text, int size=-1)
    {
        // calculate size
        if (size < 0) size = strlen(text);
        
        // the pointer is stored right in front of the name
        _pointer = *((Type **)(text - sizeof(Type *)));
        _text = text;
    }
    
    /**
     *  Copy constructor
     *  @param  that
     */
    HiddenPointer(const HiddenPointer<Type> &that) : _pointer(that._pointer), _text(that._text), _data(that._data)
    {
        // if data is filled, the text is located inside the data
        if (_data.size() > 0) _text = _data.c_str() + sizeof(Type *);
    }
    
    /**
     *  Destructor
     */
    virtual ~HiddenPointer() {}
    
    /**
     *  Assignment operator
     *  @param  that
     *  @return HiddenPointer
     */
    HiddenPointer<Type> operator=(const HiddenPointer &that)
    {
        // skip self assignment
        if (&that == this) return *this;
        
        // copy members
        _pointer = that._pointer;
        _text = that._text;
        _data = that._data;

        // if data is filled, the text is located inside the data
        if (_data.size() > 0) _text = _data.c_str() + sizeof(Type *);
    }
    
    /**
     *  Retrieve the pointer
     *  @return Type*
     */
    Type *pointer() const
    {
        return _pointer;
    }
    
    /**
     *  Change the pointer
     *  @param  Type*
     */
    void setPointer(Type *pointer)
    {
        // store pointer
        _pointer = pointer;
    
        // overwrite in data
        _data.replace(0, sizeof(Type *), (const char *)&_pointer, sizeof(Type *));
        
        // for safety reasons, we recalculate text pointer
        _text = _data.c_str() + sizeof(Type *);
    }
    
    /**
     *  Retrieve the text
     *  @return const char *
     */
    const char *text() const
    {
        return _text;
    }
    
    /**
     *  Change the text
     *  @param  text
     *  @param  size
     */
    void setText(const char *text, int size=-1)
    {
        // check if size was set
        if (size < 0) size = strlen(text);
        
        // reserve enough room for the text and the pointer
        _data.reserve(size + sizeof(Type *));
        
        // store the pointer
        _data.assign(std::string((const char *)&_pointer, sizeof(Type *)));
        
        // append the text
        _data.append(text, size);
        
        // store new text
        _text = _data.c_str() + sizeof(Type *);
    }
    
    /**
     *  Cast to the pointer
     *  @return Type*
     */
    operator Type* () const
    {
        return _pointer;
    }
    
    /**
     *  Cast to text
     *  @return const char *
     */
    operator const char * () const
    {
        return _text;
    }
    
    /**
     *  Length of the text
     *  @return int
     */
    int length() const
    {
        return _data.size() - sizeof(Type *);
    }

private:
    /**
     *  The actual pointer
     *  @var Type*
     */
    Type *_pointer;
    
    /**
     *  The original text
     *  @var text
     */
    const char *_text;
    
    /**
     *  Optional data buffer
     *  @var string
     */
    std::string _data;

};

/**
 *  End of namespace
 */
}

