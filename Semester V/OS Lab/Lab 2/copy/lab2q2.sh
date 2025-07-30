#!/bin/bash

dir=$1
pattern=$2
echo "Matching entries in $dir for pattern $pattern:"
ls $dir | grep -E $pattern
