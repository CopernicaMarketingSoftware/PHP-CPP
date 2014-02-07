/**
 *  extinterface.h
 *
 *
 *  @author Valeriy_Dmitriev <ufabiz@gmail.com>
 */

#ifndef PHPCPP_EXTINTERFACE_H_
#define PHPCPP_EXTINTERFACE_H_


/**
 *  Forward declarations
 */
struct _zend_class_entry;


/**
 *  Namespace
 */
namespace Php {


/**
 *  Class definition
 */
class ExtInterface
{
public:
    /**
     *  virtual method addSelf.
     *  Add associated with self an interface to the implementing PHP class
     *  @param  zend_class_entry*  zend_class_entry pointer
     */
    virtual void addSelf(_zend_class_entry*) = 0;

    /**
     *  Destructor
     */
    virtual ~ExtInterface() {}
    
};

/**
 *  Class definition of the concrete ExtInterface
 *  see Zend/zend_interfaces.h of php-src
 */

// zend_class_entry* == zend_ce_iterator
struct InterfaceIterator : public ExtInterface
{
    virtual void addSelf(_zend_class_entry*);
};

// zend_class_entry* == zend_ce_traversable
struct InterfaceTraversable : public ExtInterface
{
    virtual void addSelf(_zend_class_entry*);
};

// zend_class_entry* == zend_ce_aggregate
struct InterfaceAggregate : public ExtInterface
{
    virtual void addSelf(_zend_class_entry*);
};

// zend_class_entry* == zend_ce_arrayaccess
struct InterfaceArrayAccess : public ExtInterface
{
    virtual void addSelf(_zend_class_entry*);
};

// zend_class_entry* == zend_ce_serializable
struct InterfaceSerializable : public ExtInterface
{
    virtual void addSelf(_zend_class_entry*);
};


/**
 *  End of namespace Php
 */
}
#endif  /* PHPCPP_EXTINTERFACE_H_ */
