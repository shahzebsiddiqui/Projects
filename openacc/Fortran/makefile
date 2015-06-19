all: vectoradd_cpu vectoradd_gpu dgemm_cpu dgemm_gpu dgemm_gpuv1

vectoradd_cpu: makefile vectoradd_cpu.f90
	gfortran -O3 vectoradd_cpu.f90 -o $@.out
vectoradd_gpu: makefile vectoradd_gpu.f90
	pgfortran -i8 -Minfo -acc -fast -O3 -ta=nvidia vectoradd_gpu.f90 -o $@.out
vectoradd_gpu_v1: makefile vectoradd_gpu_v1.f90
	pgfortran -i8 -Minfo -acc -fast -O3 -ta=nvidia vectoradd_gpu_v1.f90 -o $@.out
dgemm_cpu: makefile dgemm_cpu.f90
	gfortran -O3 dgemm_cpu.f90 -o $@.out
dgemm_gpu: makefile dgemm_gpu.f90
	pgfortran -Minfo -acc -fast -O3 -ta=nvidia dgemm_gpu.f90 -o $@.out

clean:
	rm *.out
