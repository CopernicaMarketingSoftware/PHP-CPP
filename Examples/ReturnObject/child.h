/**
 *  Child.h
 * 
 *  Class that is exported to PHP space
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 Copernica BV
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  Class definition
 */
class Child : public Php::Base
{
public:
    /**
     *  Constructor and destructor
     */
    Child() {}
    virtual ~Child() {}
    
    /**
     *  Cast to a string
     *  @return const char *
     */
    const char *__toString() const
    {
        return "this is the child";
    }
};

