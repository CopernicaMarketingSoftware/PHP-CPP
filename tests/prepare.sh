#!/bin/bash
#
# do not run this script.
# Create a local copy of the directory with the extension for run without installation
#


EXTDLOC=ext_dir
SO=extfortest.so

if [ $1 ]
then
    SO=$1
fi

EXTDIR=$(php-config --extension-dir)

#echo $EXTDIR

mkdir -p "./$EXTDLOC"
for LIBFILE in `find $EXTDIR -type f -or -type l -name "*.so"`; do

    BN=$(basename $LIBFILE)
    PWD=$(pwd)
    NF="$PWD/$EXTDLOC/$BN"
    
    # if still no exist
    if [ ! -L $NF ];
    then
        if [ -L $LIBFILE ];
        then
            #echo "cp $LIBFILE $NF"
            cp --no-dereference $LIBFILE $NF
        else
            ln -s $LIBFILE $NF
        fi
    else
        :
    fi;
done

# current extention
if [ ! -L "$PWD/$EXTDLOC/$SO" ];
then
    ln -s "$PWD/cpp/$SO" "$PWD/$EXTDLOC/$SO"
fi
    


