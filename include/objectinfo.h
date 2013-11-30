/**
 *  objectinfo.h
 *
 *  Allows access to detailed information on the specified PHP object. Allows us
 *  to call methods, in principle, it allows us to accept PHP objects as parameters.
 *
 *  A nice article on objects within the Zend Framework can be found here:
 *  https://wiki.php.net/internals/engine/objects
 *
 *  @author Swen Kooij (Photonios) <photonios@outlook.com>
 *  @copyright 2013 Swen Kooij (Photonios)
 */

/**
 * Forward declarations because the compiler doesn't know these types yet.
 */
struct _zend_class_entry;
typedef struct _zend_class_entry zend_class_entry;

namespace Php
{

/**
 * Forward declarations because the compiler doesn't know these types yet.
 */
class Value;

/**
 * Allows access to information on the specified object, basiclly represents the
 * specified PHP object in C++.
 * @author Swen Kooij
 */
class ObjectInfo
{
public:
	/**
	 * Initializes a new instance of the ObjectInfo class, which allows access to
	 * object information on the specified object.
	 * @param value An instance of the Php::Value class, if the specified value is not
	 * 				an object, an exception will be thrown.
	 */
	ObjectInfo(const Value &value);

	/**
	 * Gets the name of the class this object is of.
	 * @returns The name of the class this object is of, if the name could not be retrieved, an empty string will be returned.
	 */
	std::string GetClassName();

	/**
	 * Gets an instance of the Php::Base class, which is the class that all of our objects
	 * inherit from.
	 *
	 * @returns A pointer to an instance of the Base class, which represents this object.
	 * 			Returns NULL if something went wrong.
	 */
	Base * GetObject();

	/**
	 * Gets the amount of references that exist to this object.
	 * @returns An integer, indicating the amount of references that exist to this object.
	 * 			Or, -1 when something went wrong.
	 */
	int GetReferenceCount();

	/**
	 * Gets the name of the file the definition of this class resides in.
	 * @returns A string containing the name of the file this class resides in,
	 * 		    or an empty string if something went wrong.
	 */
	std::string GetFilename();

	/**
	 * Gets a pointer to the underlying object by casting the base object to the specified
	 * type.
	 * @note If the base object could not be retrieved, or the base object could not be
	 *  	 converted into the specified type, NULL will be returned.
	 * @note Do not specify a pointer type in the type specifier, specify a type, a pointer
	 * 		 of the specified type will be returned.
	 * @note Free the damn pointEclipseer when you're done.
	 * @tparam The type to convert (dynamic cast) to retrieved object to.
	 * @returns A pointer to the underlying object.
	 */
	template<typename T>
	T* GetObjectPtr()
	{
		Base * base_obj = GetObject();
		if(base_obj == NULL)
			return NULL;

		return dynamic_cast<T*>(base_obj);
	}

private:
	/**
	 * The value that this instance was initialized with.
	 */
	const Value & 					m_value;
};
} // namespace Php
