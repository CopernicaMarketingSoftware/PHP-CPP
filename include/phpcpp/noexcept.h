/**
 *  NoExcept.h
 *
 *  Some compilers (hello microsoft!) do not yet support the "noexcept"
 *  keyword. To overcome this, we use a macro that expands to "noexcept"
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @author atvive <https://github.com/atvise>
 */

/**
 *  Macro to be able to support MSVC compiler
 */
#ifndef _NOEXCEPT
# ifndef _MSC_VER
#  define _NOEXCEPT noexcept
# else
#  define _NOEXCEPT __declspec(nothrow)
# endif
#endif

