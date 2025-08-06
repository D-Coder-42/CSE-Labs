#!/bin/bash

echo "Enter a number:"
read number

if [ "$number" -lt 0 ]; then
    echo "Negative"
elif [ "$number" -eq 0 ]; then
    echo "Zero"
else
    echo "Positive"
fi

