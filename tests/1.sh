#!/bin/bash
php -d enable_dl=On -d extension_dir=./ext_dir -d extension=extfortest.so -r "var_export(extension_loaded('extention_for_tests'));"

php -i | grep LD

