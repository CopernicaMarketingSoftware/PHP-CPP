/**
 *
 *  double -> string
 *
 */
#include <sstream>
#include <iomanip>
std::string double2str(long double d)
{
	std::ostringstream strs;
	strs << std::setprecision(16) << d;
	return strs.str();
}


