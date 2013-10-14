/**
 *  Members.h
 *
 *  Internal helper class that holds all members of a class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2013 Copernica BV
 */

/**
 *  Namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Members : public std::vector<Member>
{
public:
    /**
     *  Constructor
     *  @param  arguments
     */
    Members(const std::initializer_list<Member> &members) : std::vector<Member>(members), _methods(NULL) {}

    /**
     *  Destructor
     */
    virtual ~Members();

    /**
     *  Get access to the methods
     *  @param  classname
     *  @return Methods
     */
    struct _zend_function_entry *methods(const char *classname);

private:
    /**
     *  Number of methods
     *  @return integer
     */
    int methods();

    /**
     *  Array of method structures used internally in the Zend engine
     *  @var    zend_function_entry
     */
    struct _zend_function_entry *_methods;
};

/**
 *  End of namespace
 */
}

