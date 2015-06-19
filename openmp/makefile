all: openmp_matrixmux

openmp_matrixmux: makefile openmp_matrixmux.c
	gcc -o gcc_openmp_matrixmux.o openmp_matrixmux.c -fopenmp -O3
	icc -o icc_openmp_matrixmux.o openmp_matrixmux.c -fopenmp -O3 -align -no-prec-div -xSSE3

clean:
	rm -rf *.o
