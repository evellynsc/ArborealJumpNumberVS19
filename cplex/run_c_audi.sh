#!/bin/bash
instances=$(ls /home/lapo/evellyn/instances/sop/c1/*)

for f in $instances
do
  echo "Processing $f file..."
  # take action on each file. $f store current file name
  #./ajns "$f" 
  ./ajns $f 5 0 > $f.output 
done
