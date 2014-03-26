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

/**
 *  End of namespace
 */
}

