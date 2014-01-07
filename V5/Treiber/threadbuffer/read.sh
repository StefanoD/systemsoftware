#!/bin/sh

NUMS="1 2 3 4 5"

for NUM in $NUMS
do
   cat /dev/buf 
   seconds=$(( (RANDOM%10+1) ))
   echo "Read sleep $seconds s"
   sleep $seconds
done
