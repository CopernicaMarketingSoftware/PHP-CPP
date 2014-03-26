#!/bin/bash
#
# It is intended to run the command "make test" from the root directory of the library
#
#  @author valmat <ufabiz@gmail.com>
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
    echo "  -r           Removes all auxiliary files. (*.log, *.diff etc)."
    echo "  -t <files>   Test file(s). Specify individual file(s) to test"
    echo "  -s <file>    Write output to <file>."
    echo "  -x           Sets 'SKIP_SLOW_TESTS' environmental variable."
    echo "  -o           Cancels sets 'SKIP_ONLINE_TESTS' (default set)."
    echo "  -q           Quiet, no user interaction (same as environment NO_INTERACTION)."
    echo "  -v           Verbose mode."
    echo "  -h           This Help."
    echo
    exit;
}

PHP_BIN="/usr/bin/php"
# options list
SCR_OPT=""
COMPILE_EXT=1
# SKIP_ONLINE_TESTS == true
OFFLINE=1
EXT_NAME="extfortest.so"
# The file list of the tests that will be launched
TEST_FILES=""

while getopts ":p:e:nw:a:d:g:mrt:s:xoqvh" opt ;
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
        r)
            # Removes all auxiliary files. (*.log, *.diff etc).
            
            # if quiet, only delete
            if [ $(echo $SCR_OPT | grep -o "\-q") ]; then
                find ./php/phpt -type f \( \
                                           -name '*.diff' -or -name '*.exp' -or -name '*.log' -or -name '*.out' \
                                           -or -name '*.php' -or -name '*.sh' -or -name '*.mem' -or \
                                           -name '*.phpt-diff' -or -name '*.phpt-exp' -or -name '*.phpt-log' -or -name '*.phpt-out' \
                                           -or -name '*.phpt-php' -or -name '*.phpt-sh' -or -name '*.phpt-mem' \
                                        \) -delete
            
            # else delete and print
            else
                echo -e "\x1b[1;31;47mRemove auxiliary files...\x1b[0;0m"
                for AUXF in `find ./php/phpt -type f \( \
                                           -name '*.diff' -or -name '*.exp' -or -name '*.log' -or -name '*.out' \
                                           -or -name '*.php' -or -name '*.sh' -or -name '*.mem' -or \
                                           -name '*.phpt-diff' -or -name '*.phpt-exp' -or -name '*.phpt-log' -or -name '*.phpt-out' \
                                           -or -name '*.phpt-php' -or -name '*.phpt-sh' -or -name '*.phpt-mem' \
                                        \)`;
                    do
                    echo -e "\x1b[1;31;47mdel \x1b[0;30;47m$(dirname $AUXF)/\x1b[1;31;47m$(basename $AUXF)\x1b[0;30;47m ...\x1b[0;0m"
                    rm $AUXF
                done
            fi
            exit;
            ;;
        t)
            TEST_FILES="$OPTARG"
            ;;
        x)
            SCR_OPT="$SCR_OPT -x"
            ;;
        o)
            # SKIP_ONLINE_TESTS == false
            OFFLINE=0
            ;;
        q)
            SCR_OPT="$SCR_OPT -q"
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

# The file list of the tests that will be launched
if [ ! "$TEST_FILES" ]; then
    TEST_FILES=`find ./php/phpt -type f -name "*.phpt"`
fi

# default offline mode
if [ 1 = $OFFLINE ]; then
    SCR_OPT="$SCR_OPT --offline"
fi

# A list of all additional options
SCR_OPT="$SCR_OPT -d extension_dir=$PWD/ext_dir -d extension=$EXT_NAME"


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


# run tests
$PHP_BIN run-tests.php $SCR_OPT -p "$PHP_BIN" $TEST_FILES



