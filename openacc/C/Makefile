all: matrix-acc-create kernel_function mm_gpu

FLAGS= -fast -acc -O3 -Minfo-accel
matrix-acc-create: makefile matrix-acc-create.c
	pgcc -o $@.out matrix-acc-create.c $(FLAGS)
kernel_function: makefile kernel_function.c
	pgcc -o $@.out kernel_function.c  $(FLAGS)
mm_gpu: makefile mm_gpu.c
	pgcc -o $@.out $(FLAGS)
clean:
	rm -f *.out
