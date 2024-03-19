#!/bin/sh
if [ "$1" != "" ]; 
then
    echo "Compiling $1"
    ./llamac -f < $1 > a.s || exit 1
    gcc a.s ./lib/lib.a -o a.out -lm -no-pie -g
fi
