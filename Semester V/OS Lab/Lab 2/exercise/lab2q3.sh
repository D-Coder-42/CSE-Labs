#!/bin/bash

count=`who -u | wc -l`
echo "Number of users logged on to the system: $count"
