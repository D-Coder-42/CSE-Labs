#!/bin/bash

homedir=$(pwd)

for file in /*; do
    echo "$file"
done

cd "$homedir"

