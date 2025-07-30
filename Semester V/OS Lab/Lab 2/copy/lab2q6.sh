#!/bin/bash

list=`ls | grep -E ".$1"`
mkdir -p $2
cp $list $2
