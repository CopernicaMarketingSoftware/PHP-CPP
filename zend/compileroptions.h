/**
 *  CompilerOptions.h
 *
 *  Helper class to temporarily set compiler options
 *
 *  When an object is destructed, it automatically restored the previous compiler settings
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Open PHP namespace
 */
namespace Php {

/**
 *  Class definition
 */
class CompilerOptions
{
private:
    /**
     *  The original compiler options
     *  @var uint32_t
     */
    uint32_t _original;

public:
    /**
     *  Constructor
     *  @param  options
     */
    CompilerOptions(uint32_t options)
    {
        // remember the old compiler options before we set temporary compile options
        _original = CG(compiler_options);

        // we're going to evaluate only once
        CG(compiler_options) = options;
    }

    /**
     *  Destructor
     */
    virtual ~CompilerOptions()
    {
        // restore original options
        CG(compiler_options) = _original;
    }
};

/**
 *  End of namespace
 */
}
