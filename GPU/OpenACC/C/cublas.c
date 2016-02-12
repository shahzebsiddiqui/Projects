#include <openacc.h>
#include <cublas_v2.h>
#include <cuda_runtime.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int main(void)
{
  long int N = 12000;
  float *A, *B, *C; 
  float time_ms = 0;
  float alpha = 1.f, beta = 0.f;
  float perf = 0;
  
  acc_set_device_num(0,acc_device_nvidia);
  acc_init(acc_device_nvidia);
 
  cudaEvent_t start, stop;
  cublasHandle_t handle;

  cudaEventCreate(&start);
  cudaEventCreate(&stop);
 
  srand(time(NULL)); 
  A = (float*)malloc(sizeof(float)*N*N);
  B = (float*)malloc(sizeof(float)*N*N);
  C = (float*)malloc(sizeof(float)*N*N);

  for (int i = 0; i < N*N; i++)
  {
    A[i] = (float) sin(i);
    B[i] = (float) cos(i);
    //A[i] = 0.1;
    //B[i] = 0.2;
    C[i] = 0.f;
  }

  #pragma acc enter data copyin(A[0:N*N],B[0:N*N],C[0:N*N])

  cublasCreate(&handle);
  cudaEventRecord(start,0);

  // matrix multiplication with cublasSgemm routine
  #pragma acc host_data use_device(A,B,C)
  {
    cublasSgemm(handle,CUBLAS_OP_N,CUBLAS_OP_N, N, N, N, &alpha, A, N, B, N, &beta, C, N);
  }
  cudaEventRecord(stop,0);
  
  cudaEventSynchronize(stop);
  cublasDestroy(handle);
  #pragma acc exit data copyout(A[0:N*N],B[0:N*N],C[0:N*N]) 
  cudaEventElapsedTime(&time_ms,start,stop);
  unsigned long int num = N*N*N;
  perf = (float) num / (1E9*(time_ms/1E3));
  printf("Perf (GFLOPS/sec): %f \t Time(ms): %f \n",perf,time_ms);
  printf("C[0]: %f\n",C[0]);
  printf("C[%d]: %f\n",N*N-1,C[N*N-1]);
  
  free(A);
  free(B);
  free(C);
  return 0;
}
