GCC = gcc
ICC = icc
VECFLAG = -vec
NOVECFLAG = -no-vec
GCCFLAGS = -O3 -openmp -std=c99 -lm
ICCFLAGS = -O3 -openmp -std=c99 -lm

BIN = icc_laplace gcc_laplace icc_mm_novec icc_mm_vec
all: $(BIN)
icc_laplace: laplace.c
	$(ICC) $(ICCFLAGS) -o $@ $<
gcc_laplace: laplace.c
	$(GCC) $(GCCFLAGS) -o $@ $<
icc_mm_novec: mm_cpu.c
	$(ICC) -o $@ $< $(NOVECFLAG) 
icc_mm_vec: mm_cpu.c
	$(ICC) -o $@ $< $(VECFLAG)
clean:
	$(RM) $(BIN)
