all:
	make -C ./src
	cp src/serial.out .
mpi:
	make mpi -C ./src
	cp src/mpi.out .
omp:
	make omp -C ./src
	cp src/omp.out .
mpiomp:
	make mpiomp -C ./src
	cp src/mpiomp.out ./mpiomp_exp_double4_rep200.out
clean:
	rm *.out
