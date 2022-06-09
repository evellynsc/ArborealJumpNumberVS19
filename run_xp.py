import os
import subprocess

instances_dir = '/home/lapo/evellyn/instances/sop/'

for filename in os.listdir(instances_dir):
    f = os.path.join(instances_dir, filename)
    # checking if it is a file
    if os.path.isfile(f):
        p = subprocess.run(['./ajns', f, 'f'], stdout=subprocess.PIPE, universal_newlines=True)       
        with open(f + '_F.out', 'w') as output_file:
            output_file.write(p.stdout)


for filename in os.listdir(instances_dir):
    f = os.path.join(instances_dir, filename)
    # checking if it is a file
    if os.path.isfile(f):
        p = subprocess.run(['./ajns', f, 'h'], stdout=subprocess.PIPE, universal_newlines=True)       
        with open(f + '_H.out', 'w') as output_file:
            output_file.write(p.stdout)
            