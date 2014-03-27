/**
 *
 *  TestValueIterator
 *
 */

/**
 *  Set up namespace
 */
namespace TestValueIterator {

    
    void loopValue(Php::Parameters &params)
    {
        std::cout << "Array/Object contains " << params[0].size() << " items" << std::endl;
        for (auto it=params[0].begin(), itend = params[0].end(); it != itend; ++it) {
            std::cout << "["<< it->first << "]="<< it->second << std::endl;
            //std::cout << "["<< it->key() << "]="<< it->value() << std::endl;
        }
        return;
    }
    
    void loopArray(void)
    {

	    Php::Value value;
	    /*
		If we fill the array in this form, we get the following:
	    *** Error in `/usr/bin/php': double free or corruption (fasttop): 0x0000000001956d60 ***
	    value[0]        = "val0";
	    value[1] 	    = "val1";
	    value["third"]  = "val3";
	    value["fourth"] = "val3";
	    */
	    value.set(0       , "val0");
	    value.set(1 	  , "val1");
	    value.set("third" , "val3");
	    value.set("fourth", "val3");

	    std::cout << "Array/Object contains " << value.size() << " items" << std::endl;
	    // assum the value variable holds an array or object, it then
	    // is possible to iterator over the values or properties
	    for (auto &iter : value)
	    {
	        // output key and value
	        Php::out << "["<< iter.first << "]="<< iter.second << std::endl;
	    }
    }

/**
 *  End of namespace
 */
}

