#!/bin/sh
if [ "$1" != "" ]; 
then
    echo "Compiling $1"
    ./llama < $1 > a.ll || exit 1
    llc-13 a.ll -o a.s
    gcc a.s ./lib/lib.a -o a.out -no-pie -g
fi