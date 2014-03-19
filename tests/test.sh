#!/bin/bash
#
# It is intended to run the command "make test" from the root directory of the library
#


THIS=`basename $0`
    
function print_help() {
    echo "Use: $THIS [options...]"
    echo "Options:"
    echo "  -p <php>     Specify PHP executable to run (default: /usr/bin/php)."
    echo "  -e <file>    Name of test extension (default: extfortest.so)."
    echo "  -n           Do not compile the test extension."
    echo "  -w <file>    Write a list of all failed tests to <file>."
    echo "  -a <file>    Same as -w but append rather then truncating <file>."
    echo "  -d foo=bar   Pass -d option to the php binary (Define INI entry foo"
    echo "               with value 'bar')."
    echo "  -g <opt>     Comma separated list of groups to show during test run"
    echo "               (possible values: PASS, FAIL, XFAIL, SKIP, BORK, WARN, LEAK, REDIRECT)."
    echo "  -m           Test for memory leaks with Valgrind."
    echo "  -s <file>    Write output to <file>."
    echo "  -x           Sets 'SKIP_SLOW_TESTS' environmental variable."
    echo "  -o           Cancels sets 'SKIP_ONLINE_TESTS' (default set)."
    echo "  -v           Verbose mode."
    echo "  -h           This Help."
    echo
    exit;
}

PHP_BIN="/usr/bin/php"
SCR_OPT=""
COMPILE_EXT=1
OFFLINE=1
EXT_NAME="extfortest.so"

while getopts ":p:e:nw:a:d:g:ms:xovh" opt ;
do
    case $opt in
        p)
            PHP_BIN=$OPTARG;
            ;;
        e)
            EXT_NAME=$OPTARG;
            ;;
        n)
            COMPILE_EXT=0
            ;;
        w)
            SCR_OPT="$SCR_OPT -w $OPTARG"
            ;;
        a)
            SCR_OPT="$SCR_OPT -a $OPTARG"
            ;;
        d)
            SCR_OPT="$SCR_OPT -d $OPTARG"
            ;;
        g)
            SCR_OPT="$SCR_OPT -g $OPTARG"
            ;;
        s)
            SCR_OPT="$SCR_OPT -s $OPTARG"
            ;;
        m)
            SCR_OPT="$SCR_OPT -m"
            ;;
        x)
            SCR_OPT="$SCR_OPT -x"
            ;;
        o)
            OFFLINE=0
            ;;
        v)
            SCR_OPT="$SCR_OPT -v"
            ;;
        h)
            print_help
            ;;
        *)
            echo "wrong option -$OPTARG";
            echo "";
            print_help
            ;;
    esac
done

# default offline mode
if [ 1 = $OFFLINE ]; then
    SCR_OPT="$SCR_OPT --offline"
fi

TEST_FILES=`find ./php/phpt -type f -name "*.phpt"`

#RUN_SCR="$PHP_BIN -z ./cpp/$EXT_NAME"
RUN_SCR="$PHP_BIN -d enable_dl=On -d extension_dir=$PWD/ext_dir -d extension=$EXT_NAME"


# Create a local copy of the directory with the extension for run without installation
./prepare.sh $EXT_NAME


if [ 1 = $COMPILE_EXT ]; then
    echo "Compile the test extension"
    cd cpp
    make clean && make
    cd ..
fi



LD_LIBRARY_PATH="$(cd $PWD/.. && echo $PWD):${LD_LIBRARY_PATH}"
export LD_LIBRARY_PATH
echo $LD_LIBRARY_PATH

RUN_SCR="LD_LIBRARY_PATH=\"$LD_LIBRARY_PATH\" && export LD_LIBRARY_PATH && $RUN_SCR"
# run tests
$PHP_BIN run-tests.php $SCR_OPT -p "$RUN_SCR" $TEST_FILES



