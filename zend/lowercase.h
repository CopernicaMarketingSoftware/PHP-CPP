/**
 *  LowerCase.h
 *
 *  Class to temporary convert a name to lowercase
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2016 Copernica BV
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  Begin of namespace
 */
namespace Php {

/**
 *  Class definition
 */
class LowerCase
{
private:
    /**
     *  The lowercase name
     *  @var zend_string
     */
    zend_string *_name;

public:
    /**
     *  Constructor
     *  @param  name        the original name
     */
    LowerCase(zend_string *name) : 
        _name(zend_string_tolower(name)) {}
        
    /**
     *  No copy'ing or moving
     *  @param  that
     */
    LowerCase(const LowerCase &that) = delete;
    LowerCase(LowerCase &&that) = delete;

    /**
     *  Destructor
     */
    virtual ~LowerCase()
    {
        // release the data
        zend_string_release(_name);
    }
    
    /**
     *  Expose internal value
     *  @return zend_string*
     */
    zend_string *value()
    {
        // expose member
        return _name;
    }
    
    /**
     *  Cast to a zend_string
     *  @return zend_string*
     */
    operator zend_string* () 
    {
        // expose member
        return _name;
    }
};

/**
 *  End of namespace
 */
}
