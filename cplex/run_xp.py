import os
import subprocess
import psutil

def kill(proc_pid):
    process = psutil.Process(proc_pid)
    for proc in process.children(recursive=True):
        proc.kill()
    process.kill()

instances_dir = '/home/lapo/evellyn/instances/sop/'

for filename in os.listdir(instances_dir):
    f = os.path.join(instances_dir, filename)
    # checking if it is a file
    if os.path.isfile(f):
        p = subprocess.run(['./ajns', f, 'f'], stdout=subprocess.PIPE, universal_newlines=True)       
        with open(filename + '_F.out', 'w') as output_file:
            output_file.write(p.stdout)


# for filename in os.listdir(instances_dir):
#     f = os.path.join(instances_dir, filename)
#     # checking if it is a file
#     if os.path.isfile(f):
#         p = subprocess.run(['./ajns', f, 'h'], stdout=subprocess.PIPE, universal_newlines=True) 
#         try:
#             p.wait(timeout=3)
#         except subprocess.TimeoutExpired:
#             kill(p.pid)   

        # with open(f + '_H.out', 'w') as output_file:
        #     output_file.write(p.stdout)
            