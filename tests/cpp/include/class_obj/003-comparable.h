/**
 *
 *  Test Classes and objects
 *	003-comparable.phpt
 *
 */




/**
 *  Set up namespace
 */
namespace TestBaseClass {

   
    /**
     *  Test custom comparison operator
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

