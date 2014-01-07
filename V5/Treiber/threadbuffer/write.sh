#!/bin/sh

NUMS="1 2 3 4 5"

for NUM in $NUMS
do
   echo "HelloWorld" > /dev/buf 
   seconds=$(( (RANDOM%5+1) ))
   echo "Write sleep $seconds s"
   sleep $seconds
done
