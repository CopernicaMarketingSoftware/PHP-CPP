#!/bin/bash
#
# do not run this script. It is intended to run the command "make test" from the root directory of the library
#
if [ ! $1 ]
then
	echo "Required the path to executable PHP binary file"
	exit;
fi

EXT_NAME="extfortest.so"

TEST_FILES=`find ./php/phpt -type f -name "*.phpt"`
PHP_BIN=$1

#RUN_SCR="$PHP_BIN -z ./cpp/$EXT_NAME"
RUN_SCR="$PHP_BIN -d enable_dl=On -d extension_dir=./ext_dir -d extension=$EXT_NAME"


# Create a local copy of the directory with the extension for run without installation
./prepare.sh


echo "Compile the test extension"
cd cpp
make clean && make
cd ..

# run tests
$PHP_BIN run-tests.php --offline -p "$RUN_SCR" $TEST_FILES



