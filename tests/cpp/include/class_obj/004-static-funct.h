/**
 *
 *  Test Classes and objects
 *	004-static-funct.phpt
 *	test static functions
 *
 */




/**
 *  Set up namespace
 */
namespace TestBaseClass {

   
    /**
     *  Regular function
     *
     *  Because a regular function does not have a 'this' pointer,
     *  it has the same signature as static methods
     *
     *  @param  params      Parameters passed to the function
     */
    void testStaticRegFunc(Php::Parameters &params)
    {
        Php::out << "testStatic regular function"<< std::endl;
    }

    /**
     *  A very simple class that will not be exported to PHP
     */
    class testStaticPrivClass
    {
    public:
        /**
         *  C++ constructor and destructor
         */
        testStaticPrivClass() {}
        virtual ~testStaticPrivClass() {}

        /** 
         *  Static method
         *
         *  A static method also has no 'this' pointer and has
         *  therefore a signature identical to regular functions
         *
         *  @param  params      Parameters passed to the method
         */
        static void staticMethod(Php::Parameters &params)
        {
            Php::out << "testStaticPrivClass::staticMethod()"<< std::endl;
        }
    };

    /**
     *  A very simple class that will be exported to PHP
     */
    class  testStaticPubClass : public Php::Base
    {
    public:
        /**
         *  C++ constructor and destructor
         */
        testStaticPubClass() {}
        virtual ~testStaticPubClass() {}

        /** 
         *  Another static method
         *
         *  This static has exactly the same signature as the
         *  regular function and static method that were mentioned
         *  before
         *
         *  @param  params      Parameters passed to the method
         */
        static void staticMethod(Php::Parameters &params)
        {
            Php::out << "testStaticPubClass::staticMethod()"<< std::endl;
        }
    };



/**
 *  End of namespace
 */
}

