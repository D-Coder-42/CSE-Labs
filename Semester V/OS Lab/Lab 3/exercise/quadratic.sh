#!/bin/bash

echo "Form of equation: ax^2 + bx + c = 0"
read -p "Enter 'a': " a
read -p "Enter 'b': " b
read -p "Enter 'c': " c
echo

D=$((b*b - 4*a*c))
den=$((2*a))
p=4
case $D in
	0)
		root=$(echo "scale=$p; -1*$b/$den" | bc -l)
		echo "Roots of equation: ($root, $root)"
		;;
	-*)
		sqrtD=$(echo "scale=$p; sqrt($((-D)))" | bc -l)
		real=$(echo "scale=$p; -1*$b/$den" | bc -l)
		imag=$(echo "scale=$p; $sqrtD/$den" | bc -l)
		echo "Roots of equation: ($real+${imag}i, $real-${imag}i)"
		;;
	*)
		sqrtD=$(echo "scale=$p; sqrt($D)" | bc -l)
		root1=$(echo "scale=$p; (-1*$b + $sqrtD)/$den" | bc -l)
		root2=$(echo "scale=$p; (-1*$b - $sqrtD)/$den" | bc -l)
		echo "Roots of equation: ($root1, $root2)"
		;;
esac
