#!/bin/bash

count=`ls -l | grep '^-' | wc -l`
echo "Number of files in the current directory: $count"
