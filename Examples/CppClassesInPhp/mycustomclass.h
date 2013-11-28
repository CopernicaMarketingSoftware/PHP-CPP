/**
 * 	A test class for testing how to pass an object from php to CPP.
 * 
 * 
 */


/**
 * 	Used libraries.
 */

/**
 * 	Namespace.
 */
using namespace std;

class MyCustomClass : public Php::Base
{
private:
    int _x;
    
public:
    MyCustomClass()
    {
        _x = 3;
        cout << "MyCustomClass::MyCustomClass" << endl;
        cout << this << endl;
        cout << _x << endl;
    }
    
    virtual ~MyCustomClass()
    {
        cout << "MyCustomClass::~MyCustomClass" << endl;
    }

    virtual void __construct()
    {
        cout << "MyCustomClass::__construct" << endl;
    }

    virtual void __destruct()
    {
        cout << "MyCustomClass::__destruct" << endl;
    }
    
    void myMethod(Php::Parameters &params);
};
