/**
 * 	Includes
 */
#include "includes.h"
    
/**
 * 	Does some random printing.
 * 	@param Php::Parameters
 */
void MyCustomClass::myMethod(Php::Parameters &params)
{
	cout << "myMethod is called." << endl;
	cout << "_x: " << _x << endl;
	_x = params[0];
	cout << "New _x" << _x << endl;
}
