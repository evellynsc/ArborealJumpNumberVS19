## Set up configuration
The tool is setted up using a json file formatted as described below.
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
### Format of input file

#### 1. Main configurations
---
Configuration | Description | value type
---|---|---
infile_name | Input file name of an instance in a valid format. Valid formats are m, mtx, p.| string
algo | Algorithm configuration to solve an instance of AJNP. See [more](#2-algorithm-configuration).| dictionary
time_limit | Total time the solver is allowed to execute in seconds. The execution is stopped when time limit is reached. | integer
memnory_limit | Total memory the solver is allowed to use in MB. The execution is stopped when memory limit is reached | integer
num_threads | Total number os threads the solver is allowed to use. | integer
outfile_name | Output file name in a valid format. | string
---

#### 2. Algorithm configuration
The algorithm configuration is setted up using a dictionary as described below
---
Configuration | Description | value type
---|---|---
type | Algorithm to solve the ajnp. Valid options are MULTIFLOW, RANDOM_HEURISTIC, ORDERED_HEURISTIC, CHARACTERIZATION. | dictionary
options | Determine the options to use for the algorithm | dictionary

4. 
