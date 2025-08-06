#!/bin/bash

read -p "Enter a number: " n 

i=1
while [ $i -le $((n*2)) ]
do
	echo $i
	i=$((i+2))
done
