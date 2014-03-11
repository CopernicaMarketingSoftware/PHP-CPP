/**
 *  valuepair.h
 *
 *  ValuePair is a simple type that is used only in conjunction with ValueIterator.
 *  Intended for obtaining access to the keys, values, and types of key variable of type Value during iteration
 *
 *  @copyright 2013 Copernica BV
 */
#ifndef VALUEPAIR_H
#define VALUEPAIR_H

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class ValuePair
{
public:

    //return _value;
    Value value() const {
        return Value(*_value);
    }

    //return string key
    std::string key() const {
        return isString ? _key : std::to_string(_ind);
    }

    //return integer key (index)
    unsigned long ind() const {
        return _ind;
    }

    // key type is string?
    bool isstr() const {
        return isString;
    }

    // debug function.
    const char *typestr() const {return isString ? "str" : "num";}

private:
    
    /**
     *  Constructor.
     *  only ValueIterator can create object of ValuePair
     */
    ValuePair(): _ind(0), _key(""), isString(true), _value(nullptr) {}

    /**
     *  Reset to empty.
     *  If ValuePair oject is empty, then the containing it ValueIterator object == ValueIterator::null
     */
    void reset() {
        _ind = 0;
        _key = "";
    }

    unsigned long _ind;
    
    const char*   _key;

    //zval** _value;
    struct _zval_struct** _value;

    // key type is string?
    bool isString;

friend class ValueIterator;
};

/**
 *  End of namespace
 */
}
#endif /* valuepair.h */
