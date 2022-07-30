#main: main.o csv_reader.o matrix_stuff.o model_stuff.o
#	mpic++ -o program main.o csv_reader.o matrix_stuff.o
test: main.o csv_reader.o matrix_stuff.o model_stuff.o
	mpic++ -o program main.o csv_reader.o matrix_stuff.o
main.o: main.cpp csv_reader.cpp
	mpic++ -c main.cpp csv_reader.cpp
csv_reader.o: csv_reader.cpp
	mpic++ -c csv_reader.cpp
model_stuff.o: model_stuff.cpp
	mpic++ -c model_stuff.cpp matrix_stuff.cpp
matrix_stuff.0: matrix_stuff.cpp
	mpic++ -c matrix_stuff.cpp
cl:
	rm -f test1.o main.o csv_reader.o matrix_stuff.o model_stuff.o
1: program
	mpirun -np 1 program table1.csv 50000
	mpirun -np 2 program table1.csv 50000
	mpirun -np 3 program table1.csv 50000

