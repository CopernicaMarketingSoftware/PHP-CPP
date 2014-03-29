/**
 *
 *  double -> string
 *
 */



char num2char(unsigned int num) 
{
	switch(num)
	{
	    case 0:
	        return '0';
	    case 1:
	        return '1';
	    case 2:
	        return '2';
	    case 3:
	        return '3';
	    case 4:
	        return '4';
	    case 5:
	        return '5';
	    case 6:
	        return '6';
	    case 7:
	        return '7';
	    case 8:
	        return '8';
	    case 9:
	        return '9';
	}

	//return '\0';
	return '-';
}

std::string double2str(long double D)
{
	int sign = (D > 0) ? 1 : (D*=-1.0, -1);
	unsigned long long int Ceil = D;
	
	// if D is ceil
	if(Ceil == D)
	{
		return std::to_string( (long long)(D*sign) );
	}

	// size of result buffer
	const int bs = 32;
	
	// size of temporary buffer
	const int pw = 16;
	// Temporary buffer
	char buf[pw];
	// Result buffer
	char rez[bs];

	int i, size = 0;
	// set sign
	if(sign < 0) rez[size++] = '-';

	// set ceil
	std::string sceil = std::to_string(Ceil);
	const char * bceil = sceil.c_str();
	int sceillen = sceil.size();
	for(i = 0; i < sceillen; i++)
	{
		rez[size++] = bceil[i];
	}

	// set point
	rez[size++] = '.';
	
	unsigned long long int I =  D * 10000000000000000; // D * 10**pw
	// .14159265359 -> 14159265359000000
	I -= Ceil * 10000000000000000;

	// Remove the tail of zeros
	// 14159265359000000 -> 14159265359
	while(0 == I % 10) I /= 10;

	int ind = 0;
	while(I > 0)
	{
		buf[ind++] = num2char(I%10);
		I = (I - I%10) / 10;
	}

	// set fraction part
	for(i = 0; i < ind; i++)
	{
		rez[size] = buf[ind-i-1];
		size++;
	}

	return std::string(rez, size);
	//rez[size] = '\0';
}


