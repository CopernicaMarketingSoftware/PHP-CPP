/**
 *  extinterface.cpp
 *  
 *  
 *
 *  @author Valeriy_Dmitriev <ufabiz@gmail.com>
 */

#include "includes.h"


/**
 *  Namespace Php
 */
namespace Php {

	/**
	 *  see extinterface.h
	 */
	void InterfaceIterator::addSelf(_zend_class_entry *ce) {
		zend_class_implements(ce TSRMLS_DC, 1, zend_ce_iterator);
	}
	void InterfaceTraversable::addSelf(_zend_class_entry *ce) {
		zend_class_implements(ce TSRMLS_DC, 1, zend_ce_traversable);
	}
	void InterfaceAggregate::addSelf(_zend_class_entry *ce) {
		zend_class_implements(ce TSRMLS_DC, 1, zend_ce_aggregate);
	}
	void InterfaceArrayAccess::addSelf(_zend_class_entry *ce) {
		zend_class_implements(ce TSRMLS_DC, 1, zend_ce_arrayaccess);
	}
	void InterfaceSerializable::addSelf(_zend_class_entry *ce) {
		zend_class_implements(ce TSRMLS_DC, 1, zend_ce_serializable);
	}

	
/**
 *  End of namespace Php
 */
}
