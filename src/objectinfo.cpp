/**
 *  objectinfo.cpp
 *
 *  Allows access to detailed information on the specified PHP object. Allows us
 *  to call methods, in principle, it allows us to accept PHP objects as parameters.
 *
 *  @author Swen Kooij (Photonios) <photonios@outlook.com>
 *  @copyright 2013 Swen Kooij (Photonios)
 */

#include "includes.h"

/**
 * Somehow some of the Zend macro's didn't work, so I wrote my own. Bit dirty,
 * but it works fine.
 */
#define GET_CLASS_ENTRY(zval_p) zend_objects_get_address(zval_p TSRMLS_CC)->ce

namespace Php
{

ObjectInfo::ObjectInfo(const Value &value) :
	m_value(value)
{
	if(!value.isObject())
		throw std::runtime_error("Unable to initialize ObjectInfo, specified type is not an object.");
}

int ObjectInfo::GetReferenceCount()
{
	zend_class_entry *class_entry = GET_CLASS_ENTRY(m_value._val);

	if(class_entry == NULL)
		return -1;

	return class_entry->refcount;
}

std::string ObjectInfo::GetFilename()
{
	zend_class_entry *class_entry = GET_CLASS_ENTRY(m_value._val);

	if(class_entry == NULL)
		return std::string("");

	return std::string(class_entry->info.user.filename);
}

std::string ObjectInfo::GetClassName()
{
	zend_class_entry *class_entry = GET_CLASS_ENTRY(m_value._val);

	if(class_entry == NULL)
		return std::string("");

	std::string class_name(class_entry->name);
	return class_name;
}

Base * ObjectInfo::GetObject()
{
	MixedObject *mixed_object = (MixedObject*) zend_object_store_get_object(m_value._val TSRMLS_CC);
	if(mixed_object == NULL)
		return NULL;

	return mixed_object->cpp;
}


} // namespace Php
