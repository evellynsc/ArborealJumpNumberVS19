#! /bin/bash

# run small instances
for file in instances/small*;
do
    ./ajns.bin "$file" 0 0 10 > /output/"$file"
done

# run median instances
# for file in instances/median*;
# do
#     ./ajns.bin "$file" 0 0 3 > /output/"$file"
# done

# run big instances
# for file in instances/big*;
# do
#     ./ajns.bin "$file" 0 0 3 > /output/"$file"
# done

# run huge instances
# for file in instances/huge*;
# do
#     ./ajns.bin "$file" 0 0 3 > /output/"$file"
# done
