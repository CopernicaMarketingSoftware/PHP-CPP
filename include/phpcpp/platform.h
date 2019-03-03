/**
 *  Platform.h
 *
 *  Macro that we use to find out whether we run on 64bit or 32bit
 *  platforms.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 Copernica BV
 */

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define PHPCPP_64BIT
#else
#define PHPCPP_32BIT
#endif
#endif

// Check GCC and clang
#if __GNUC__ || __clang__
#if __x86_64__ || __ppc64__
#define PHPCPP_64BIT
#else
#define PHPCPP_32BIT
#endif
#endif
