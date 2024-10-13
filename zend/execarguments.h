/**
 *  ExecArguments.h
 * 
 *  Helper class that we use to turn an array of Value objects into an
 *  array of zval parameters
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2024 Copernica BV
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
class ExecArguments
{
private:
    /**
     *  Short-array-optimization (most exec calls do not have more than 10 parameters)
     *  @var zval[]
     */
    zval _preallocated[10];
    
    /**
     *  The actual arguments
     *  @var zval[]
     */
    zval *_argv;
    
    /**
     *  The number of arguments
     *  @var size_t
     */
    size_t _argc;
    
public:
    /**
     *  Default constructor
     */
    ExecArguments() : _argv(_preallocated), _argc(0) {}

    /**
     *  Constructor
     *  @param  argc
     *  @param  argv
     */
    ExecArguments(size_t argc, Value argv[]) : _argv(_preallocated), _argc(argc)
    {
        // if there are too many arguments, we allocate them right away
        if (_argc > 10) _argv = (zval *)malloc(sizeof(zval) * _argc);
        
        // convert Value objects to zval array with references
        for (size_t i = 0; i < argc; ++i)
        {
            // make sure the original variable is a reference so that our copy points to the same data
            // @todo not sure if this is needed, do we need to turn the parameters into references to allow for pass-by-reference parameters?
            //if (!Z_ISREF_P(argv[i]._val)) ZVAL_MAKE_REF(argv[i]._val);
            
            // copy the zval
            ZVAL_COPY(&_argv[i], argv[i]._val);
        }
    }
    
    /**
     *  No copying (we could implement this later, but for now this is not needed)
     *  @param that
     */
    ExecArguments(const ExecArguments &that) = delete;
    
    /**
     *  Destructor
     */
    virtual ~ExecArguments()
    {
        // destruct all zval objects
        for (size_t i = 0; i < _argc; ++i) zval_ptr_dtor(&_argv[i]);
    
        // deallocate memory
        if (_argv != _preallocated) free(_argv);
    }
    
    /**
     *  Convert to a argv[]
     *  @return zval[]
     */
    zval *argv() { return _argv; }
    int argc() { return _argc; }
};

/**
 *  End of namespace
 */
}
