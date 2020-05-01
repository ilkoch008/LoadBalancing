# Load Balancing
This repository presents implementations of dynamic and static balancing using the addition of two long numbers as an example.

### Parameters
```
mpirun -n [num of procs] .../sumOfBigNumbers [input file] [output file] [mode of work] [[size of block for dynamic mode]]
```
where
```
[mode of work] = "dynamic" or "static"
```
and [size of block for dynamic mode] is unit workload for slave processes.

[size of block for dynamic mode] it is not necessary to enter if you runs program in static mode.
### Format of input file
```
[number >= number of digits in biggest number]
[first number]
[second number]
```
