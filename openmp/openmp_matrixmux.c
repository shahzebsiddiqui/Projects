#include <stdio.h>
#include "timer.h"
#include <omp.h>
#define SIZE 1000 

float a[SIZE][SIZE];
float b[SIZE][SIZE];
float c[SIZE][SIZE];

int main()
{
 
  int threadcnt;
  int i,j,k;
  printf("Computing Matrix Multiplication C = A * B with %dx%d square matrix\n",SIZE,SIZE);
  for (threadcnt = 1; threadcnt <= 16; threadcnt++)
 {
  StartTimer();
  omp_set_num_threads(threadcnt);
  // Initialize matrices.
   #pragma omp parallel for
   for (i = 0; i < SIZE; ++i) {
    for (j = 0; j < SIZE; ++j) {
      a[i][j] = (float)i + j;
      b[i][j] = (float)i - j;
      c[i][j] = 0.0f;
    }
  }
  // Compute matrix multiplication.
  #pragma omp parallel for
  for (i = 0; i < SIZE; ++i) {
    for (j = 0; j < SIZE; ++j) {
      for (k = 0; k < SIZE; ++k) {
	c[i][j] += a[i][k] * b[k][j];
      }
    }
  }

  double runtime = GetTimer();
  #pragma omp parallel
  {
    if(omp_get_thread_num() == 0)
	printf("Execution Time: %f s with %d threads\n", runtime / 1000.f,omp_get_num_threads());
  }
 }
  /*
  for (i = 0; i < SIZE; i++)
  {
    for (j = 0; j < SIZE; j++)
	printf("%0.6f\t",c[i][j]);
	printf("\n");
  }*/
  return 0;
}
