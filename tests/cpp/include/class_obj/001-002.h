/**
 *
 *  Test Classes and objects
 *	001.phpt
 *	002.phpt
 *
 */




/**
 *  Set up namespace
 */
namespace TestBaseClass {


    class MyCustomClass : public Php::Base, public Php::Countable
    {
    private:
        int _x = 3;
        
    public:
        MyCustomClass()
        {
            std::cerr << "MyCustomClass::MyCustomClass()" << std::endl;
        }

        MyCustomClass(int value) : _x(value)
        {
            std::cerr << "MyCustomClass::MyCustomClass(" << value << ")" << std::endl;
        }

        MyCustomClass(const MyCustomClass &that)
        {
            //std::cerr << "MyCustomClass::MyCustomClass copy constructor" << std::endl;
        }
        
        virtual ~MyCustomClass()
        {
            std::cerr << "MyCustomClass::~MyCustomClass" << std::endl;
        }

        virtual long int count() override
        {
            return 33;
        }

        Php::Value myMethod(Php::Parameters &params)
        {
            // check number of parameters
            //if (params.size() != 1) throw Php::Exception("Invalid number of parameters supplied");
            
            Php::out << "myMethod is called for object " << _x << std::endl;

            return 5;

        }
    };


/**
 *  End of namespace
 */
}

