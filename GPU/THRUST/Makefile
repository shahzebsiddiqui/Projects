CC = nvcc
CFLAGS = -O3
BIN = cublas grades vecAdd vecOp_tuple

all: $(BIN)

cublas: cublas.cu
	$(CC) $(CFLAGS) -o $@ $@.cu -lcublas

grades:	grades.cu
	$(CC) $(CFLAGS) -o $@ $@.cu 

vecAdd: vecAdd.cu
	$(CC) $(CFLAGS) -o $@ $@.cu

vecOp_tuple: vecOp_tuple.cu
	$(CC) $(CFLAGS) -o $@ $@.cu

clean:
	rm -rf $(BIN)
