all: matrix-acc-create kernel_function mm_gpu

FLAGS= -fast -acc -O3 -Minfo=accel
matrix-acc-create:  matrix-acc-create.c
	pgcc -o $@.out matrix-acc-create.c $(FLAGS)
kernel_function:  kernel_function.c
	pgcc -o $@.out kernel_function.c  $(FLAGS)
mm_gpu:  mm_gpu.c
	pgcc -o $@.out $(FLAGS) mm_gpu.c
clean:
	rm -f *.out
