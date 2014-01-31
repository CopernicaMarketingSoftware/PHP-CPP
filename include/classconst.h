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
    ClassConst(const char *name) : Member(name, Flag(Zend::AccMemb::CONSTANT)) {}
    ClassConst(const char *name, std::nullptr_t value) : Member(name, Flag(Zend::AccMemb::CONSTANT), value) {}
    ClassConst(const char *name, int value) : Member(name, Flag(Zend::AccMemb::CONSTANT), value) {}
    ClassConst(const char *name, long value) : Member(name, Flag(Zend::AccMemb::CONSTANT), value) {}
    ClassConst(const char *name, bool value) : Member(name, Flag(Zend::AccMemb::CONSTANT), value) {}
    ClassConst(const char *name, char value) : Member(name, Flag(Zend::AccMemb::CONSTANT), value) {}
    ClassConst(const char *name, const std::string &value) : Member(name, Flag(Zend::AccMemb::CONSTANT), value) {}
    ClassConst(const char *name, const char *value, int size=-1) : Member(name, Flag(Zend::AccMemb::CONSTANT), value, size) {}
    ClassConst(const char *name, double value) : Member(name, Flag(Zend::AccMemb::CONSTANT), value) {}

    /**
     *  Constructor
     *  @param  name        Name of the property
     *  @param  method      Method to add
     *  @param  arguments   Optional argument information
     */
    ClassConst(const char *name, const _Method &method, const std::initializer_list<Argument> &arguments = {}) : Member(name, Flag(Zend::AccMemb::CONSTANT), method, arguments) {}

    /**
     *  Destructor
     */
    virtual ~ClassConst() {}
   
};


/**
 *  End of namespace
 */
}

