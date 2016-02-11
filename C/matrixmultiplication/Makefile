
ICC = icc
VECFLAG = -vec
NOVECFLAG = -no-vec
O1=-O1
O2=-O2
O3=-O3 

BIN = icc_mm_novec icc_mm_vec icc_mm_o1 icc_mm_o2 icc_mm_o3
all: $(BIN)
icc_mm_novec: mm_cpu.c
	$(ICC) -o $@ $< $(NOVECFLAG) 
icc_mm_vec: mm_cpu.c
	$(ICC) -o $@ $< $(VECFLAG) $(O3)
icc_mm_o1: mm_cpu.c
	$(ICC) -o $@ $< $(01)
icc_mm_o2: mm_cpu.c
	$(ICC) -o $@ $< $(02)
icc_mm_o3: mm_cpu.c
	$(ICC) -o $@ $< $(03)
clean:
	$(RM) $(BIN)
