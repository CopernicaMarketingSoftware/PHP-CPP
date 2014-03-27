/**
 *  
 *
 *  Classes_and_objects.h
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
            
            std::cout << "myMethod is called for object " << _x << std::endl;

            return 5;

        }
    };


    /**
     *  custom comparison operator
     */
    class Comparable : public Php::Base
    {
    private:
        /**
         *  Internal value of the class
         *  @var    int
         */
        static int count;
        int _nom;
        int _value;

    public:
        /**
         *  C++ constructor
         */
        Comparable() 
        {
            // start with random value
            //_value = rand();
            _nom   = ++count;
            _value = _nom%2+1;
        }
        
        /**
         *  C++ destructor
         */
        virtual ~Comparable() {}

        /**
         *  Cast the object to a string
         *  @return std::string
         */
        std::string __toString()
        {
            return "Obj#" + std::to_string(_nom) + "(" + std::to_string(_value) + ")";
        }
        
        /**
         *  Compare with a different object
         *  @param  that
         *  @return int
         */
        int __compare(const Comparable &that) const
        {
            return _value - that._value;
        }
    };
    int Comparable::count = 0;






/**
 *  End of namespace
 */
}

    