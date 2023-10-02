#!/bin/bash

for f in /home/lapo/evellyn/instances/sop/*ajn.txt
do
  echo "Processing $f file..."
  # take action on each file. $f store current file name
  ./ajns "$f" 'd' 'lr' > "${f}.output" 
done
