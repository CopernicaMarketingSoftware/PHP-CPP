#!/bin/bash
#
#  do not run this script manually.
#  This script is intended to be run from a script test.sh
#  Create a local copy of the directory with the extension for run tests without installation
#
#  @author valmat <ufabiz@gmail.com>
#

# Local directory in which to copy the links to all installed extensions
EXTDLOC=ext_dir
mkdir -p "./$EXTDLOC"

SO=extfortest.so

# to check whether specified the extension name as parameter
if [ $1 ]
then
    SO=$1
fi

# a directory of extensions already installed in the operating system
EXTDIR=$(php-config --extension-dir)

# To copy references for all extensions to the local directory
for LIBFILE in `find $EXTDIR -type f -or -type l -name "*.so"`; do

    BN=$(basename $LIBFILE)
    NF="$PWD/$EXTDLOC/$BN"
    
    # if still no exist
    if [ ! -L $NF ];
    then
        if [ -L $LIBFILE ];
        then
            cp --no-dereference $LIBFILE $NF
        else
            ln -s $LIBFILE $NF
        fi
    else
        :
    fi;
done

# current extention
if [ ! -L "$PWD/$EXTDLOC/$SO" ]
then
    ln -s "$PWD/cpp/$SO" "$PWD/$EXTDLOC/$SO"
fi
    

