### Set up configuration
```json
{
    "infile_name": "arquivo.m",
    "algo": {"type": "CHARACTERIZATION", "options": {"relaxed": false,"order": "ASC"}},
    "time_limit": 3600,
    "memory_limit": "40000",
    "num_threads": 4,
    "outfile_name": ""
}
```
#### Format of input file
---
Configuration | Description | value type
---|---|---
infile_name | Input file name in a valid format. Valid formats are m, mtx, p.| string
algo | Algorithm to solve the ajnp. Valid options are MULTIFLOW, RANDOM_HEURISTIC, ORDERED_HEURISTIC, CHARACTERIZATION. | dict
time_limit | Total time the solver is allowed to execute in seconds. The execution is stopped when time limit is reached. | integer
memnory_limit | Total memory the solver is allowed to use in MB. The execution is stopped when memory limit is reached | integer
num_threads | Total number os threads the solver is allowed to use. | integer
outfile_name | Output file name in a valid format. | string
---
