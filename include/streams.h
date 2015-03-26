/**
 *  Streams.h
 *
 *  Just like the standard std::cout and std::cerr objects to output data, you
 *  can use similar stream objects for outputting data to PHP. Php::out is the
 *  C++ equivalent of the PHP echo() function, and Php::err() is the C++ stream
 *  that behaves like calling trigger_error() from PHP.
 *
 *  Php::out << "this is example text" << std::endl;
 *  Php::err << "this is an error message" << std::endl;
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Define the out and err objects
 */
extern PHPCPP_EXPORT std::ostream out;
extern PHPCPP_EXPORT std::ostream error;
extern PHPCPP_EXPORT std::ostream notice;
extern PHPCPP_EXPORT std::ostream warning;
extern PHPCPP_EXPORT std::ostream deprecated;

/**
 *  End namespace
 */
}

