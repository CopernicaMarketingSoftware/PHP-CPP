dnl PHP_REQUIRE_CXX()
dnl PHP_ADD_LIBRARY(stdc++, 1, PHP5CPP_SHARED_LIBADD)
PHP_NEW_EXTENSION(phpcpp, extension.cpp, $ext_shared,,"","yes")
    
