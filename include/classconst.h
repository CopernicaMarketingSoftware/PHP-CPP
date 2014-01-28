/**
 *  ClassConst.h
 *
 *  Class for adding `class constant` to a class
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  Class definition
 */
class ClassConst : public Member
{
public:
    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  value       Default value of the property
     */
    ClassConst(const char *name) : Member(name, true) {isClassConst(true); std::cout << "ClassConst Constructor NULL:" << name << std::endl;}
    ClassConst(const char *name, std::nullptr_t value) : Member(name, true, value) {isClassConst(true); std::cout << "ClassConst Constructor: nullptr_t " << name << std::endl;}
    ClassConst(const char *name, int value) : Member(name, true, value) {isClassConst(true); std::cout << "ClassConst Constructor: int " << name << std::endl;}
    ClassConst(const char *name, long value) : Member(name, true, value) {isClassConst(true); std::cout << "ClassConst Constructor: long " << name << std::endl;}
    ClassConst(const char *name, bool value) : Member(name, true, value) {isClassConst(true); std::cout << "ClassConst Constructor: bool " << name << std::endl;}
    ClassConst(const char *name, char value) : Member(name, true, value) {isClassConst(true); std::cout << "ClassConst Constructor: char" << name << std::endl;}
    ClassConst(const char *name, const std::string &value) : Member(name, true, value) {isClassConst(true); std::cout << "ClassConst Constructor: string " << name << std::endl;}
    ClassConst(const char *name, const char *value, int size=-1) : Member(name, true, value, size) {isClassConst(true); std::cout << "ClassConst Constructor: char * " << name << std::endl;}
    ClassConst(const char *name, double value) : Member(name, true, value) {isClassConst(true); std::cout << "ClassConst Constructor double:" << name << std::endl;}

    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  method      Method to add
     *  @param  arguments   Optional argument information
     */
    ClassConst(const char *name, const _Method &method, const std::initializer_list<Argument> &arguments = {}) : Member(name, true, method, arguments) {isClassConst(true); std::cout << "ClassConst Constructor:" << name << std::endl;}

    /**
     *  Destructor
     */
    virtual ~ClassConst() {}
 
    
};

/**
 *  End of namespace
 */
}

