#!/bin/bash

SECONDS=0
value=0

while [ $value -le 4294967295 ] ; do
  value=$(( $value + 1 ))
  duration=$SECONDS
  min=$(($duration / 60))
  sec=$(($duration % 60))
#  echo "$min minutes and $sec seconds elapsed."
#  echo "The program has been running $value times so far."
  echo $value >> I_Guess
done
