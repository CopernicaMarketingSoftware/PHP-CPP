/**
 *  thread_local.h
 *
 *  Some compilers claim to be C++11 compliant, yet fail to
 *  implement basic features, such as the thread_local storage
 *  specifier. These little cripples we help out a bit by
 *  redefining the keyword to whatever proprietary cruft
 *  they are using.
 *
 *  @copyright 2016 Copernica B.V.
 */

// are we dealing with an outdated sort of compiler?
#if defined(_MSC_VER) && _MSC_VER < 1500
    // can't use thread_local, but we can use their
    // limited storage scope specifier in this case
    #define thread_local __declspec( thread )
#endif
