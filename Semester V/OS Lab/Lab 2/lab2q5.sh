#!/bin/bash

echo "Numbers in sorted order: "
res=`cat "$1" "$2" | sort -g | uniq`
echo "$res" > result
