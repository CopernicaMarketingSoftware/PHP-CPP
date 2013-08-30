/**
 *	HiddenPointer.h
 *
 *	Helper class that we use to hide a pointer in a string. We do this
 *	by creating a string buffer that is a littlebit bigger, and put
 *	the hidden pointer in front of the name
 *
 *	@author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *	@copyright 2013 Copernica BV
 */

/**
 *	Set up namespace
 */
namespace PhpCpp {

/**
 *	Class definition
 */
template <typename Type>
class HiddenPointer
{
public:
	/**
	 *	Constructor to hide the pointer in a buffer
	 *	@param	pointer		The hidden pointer
	 *	@param	text		The visible text
	 *	@param	size		Optional text size
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
	 *  Constructor to retrieve the object given a buffer
	 *  @param	text		The visible text
	 * 	@param	size		Size of the text
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
	 * 	@param	that
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
	 *  @param	that
	 * 	@return	HiddenPointer
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
	Type *pointer()
	{
		return _pointer;
	}
	
	/**
	 *  Retrieve the text
	 *  @return const char *
	 */
	const char *text()
	{
		return _text;
	}
	
	/**
	 *  Cast to the pointer
	 *  @return Type*
	 */
	operator Type* ()
	{
		return _pointer;
	}
	
	/**
	 *  Cast to text
	 *  @return const char *
	 */
	operator const char * ()
	{
		return _text;
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
 *	End of namespace
 */
}

