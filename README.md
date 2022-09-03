# gmdh

Build a function that matches data values with a minimal external criterion. Currently supports only k + b1x1 + b2x2 + b3*x1^2

requirements for the project: openmpi 4.1.4

compile: make in directory

start: mpirun -np (amount of cores) program (data) (amount of columns to read)
example: mpirun -np 4 program table1.csv 900000

check tables in repo to see the example of how the data should be arranged
