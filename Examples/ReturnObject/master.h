/**
 *  Master.h
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
 *  Dependencies
 */
#include "child.h"

/**
 *  Class definition
 */
class Master : public Php::Base
{
private:
    /**
     *  One child
     *  @var Php::Value
     */
    Php::Value _value;

public:
    /**
     *  Constructor
     */
    Master() 
    {
        // create a child instance
        _value = Php::Object("Child", new Child());
    }
    
    /**
     *  Destructor
     */
    virtual ~Master() {}
    
    /**
     *  Retrieve the child
     *  @return Php::Value
     */
    Php::Value child() const
    {
        return _value;
    }
    
    /**
     *  Cast to a string
     *  @return const char *
     */
    const char *__toString() const
    {
        return "this is the master";
    }
};

