#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/copy.h>
#include <thrust/random.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>
using namespace std;

int main(void)
{
  int N = 10000;
  thrust::host_vector<float> hA(N*N);
  thrust::host_vector<float> hB(N*N);
  thrust::host_vector<float> hC(N*N);

  thrust::default_random_engine rng;
  thrust::uniform_int_distribution<float> dist(0,128);
  
   for (int i =0; i < N*N; i++)
   {
      hA[i] = dist(rng);
      hB[i] = dist(rng);
   }
   
 
  thrust::device_vector<float> dA(N*N);
  thrust::device_vector<float> dB(N*N);
  thrust::device_vector<float> dC(N*N);
 
  dA = hA;
  dB = hB;
 
  float alpha = 1;
  float beta = 0;

  float * raw_dA, *raw_dB, *raw_dC;
  raw_dA = thrust::raw_pointer_cast(&dA[0]);
  raw_dB = thrust::raw_pointer_cast(&dB[0]);
  raw_dC = thrust::raw_pointer_cast(&dC[0]);

  
  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
 
  cublasHandle_t handle;
  cublasCreate(&handle);
  
  cudaEventRecord(start,0);
  cublasSgemm(handle,CUBLAS_OP_N,CUBLAS_OP_N, N, N, N, &alpha, raw_dA, N, raw_dB, N, &beta, raw_dC, N);
  cudaEventRecord(stop,0);
  cudaEventSynchronize(stop);
  float time_ms = 0.0;
  cudaEventElapsedTime(&time_ms,start,stop);

  cublasDestroy(handle);
  hC = dC;
  //thrust::copy(hC.begin(),hC.end(),ostream_iterator<float>(cout,"\n"));  
  float GFLOPS = (float)N * N * N/(1E9 * (time_ms/1E3));
  printf("Matrix Multiplication - CUBLAS 10000 x 10000\n");
  printf("GFLOPS/sec = %f \n",GFLOPS);
  printf("Time(ms): %f \t \n", time_ms);
  return 0;
}
