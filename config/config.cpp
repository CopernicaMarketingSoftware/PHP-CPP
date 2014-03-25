/**
 *  Config.cpp
 *
 *  Simple programs that creates the config file for PHP-CPP. PHP-CPP needs
 *  a different config file when it is installed on a system with multi-threaded
 *  PHP, and on a system with single threaded PHP.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include <iostream>
#include <php_config.h>

/**
 *  Main procedure
 *  @return int
 */
int main()
{
#ifdef ZTS
    // also define ZTS in the config file
    std::cout << "#define ZTS" << std::endl;
#endif

    // done
    return 0;
}

