/**
 *  Init.h
 *
 *  Variables and structured required by the Zend engine to work
 *  with global variables
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  The way how PHP C API deals with "global" variables is peculiar.
 * 
 *  The following macros are supposed to turn into a structure that is going 
 *  to be instantiated for each parallel running request, and for which the 
 *  PHP engine allocates a certain amount of memory, and a magic pointer that 
 *  is passed and should be forwarded to every thinkable PHP function.
 * 
 *  We don't use this architecture. We have our own environment object
 *  that makes much more sense, and that we use. However, the Zend engine
 *  expects this structure and this structure to exist.
 */
ZEND_BEGIN_MODULE_GLOBALS(phpcpp)
ZEND_END_MODULE_GLOBALS(phpcpp)

/**
 *  And now we're going to define a macro. This also is a uncommon architecture 
 *  from PHP to get access to a variable from the structure above.
 */
#ifdef ZTS
#define PHPCPP_G(v) TSRMG(phpcpp_globals_id, phpcpp_globals *, v)
#else
#define PHPCPP_G(v) (phpcpp_globals.v)
#endif

/**
 *  We're almost there, we now need to declare an instance of the
 *  structure defined above (if building for a single thread) or some
 *  sort of impossible to understand magic pointer-to-a-pointer (for
 *  multi-threading builds). We make this a static variable because
 *  this already is bad enough.
 */
extern ZEND_DECLARE_MODULE_GLOBALS(phpcpp)

/**
 *  End of namespace
 */
}

