#!/bin/bash
instances_dir = '/home/lapo/evellyn/instances/sop'

for f in $instances_dir/*
do
  echo "Processing $f file..."
  # take action on each file. $f store current file name
  ./ajns "$f" 'b' > $f.output & 
done