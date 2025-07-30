#!/bin/bash

echo "Numbers in sorted order: "
echo `cat $1 $2 | sort -g | uniq`
