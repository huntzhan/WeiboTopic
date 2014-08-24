#!/bin/sh

file="run.log"
count=$(grep -c "###Blogs in One Table After Filtering" $file)
grep "###Blogs in One Table After Filtering" $file | cut -d ":" -f 2 | tr -d " " | awk -F'/' '{a+=$1; b+=$2;}END{print a; print b; print a/b}'