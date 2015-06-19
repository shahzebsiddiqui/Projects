#include <openacc.h>
#include <time.h>
/* matrix-acc-create.c */
#define SIZE 3000
double a[SIZE][SIZE];
double b[SIZE][SIZE];
double c[SIZE][SIZE];
 
int main()
{
  int i,j,k;
  double begin,end;

  acc_set_device_num(0,acc_device_nvidia);
  begin = clock(); 
  #pragma acc kernels create(a,b) copyout(c)
  { // start of kernels
    // Initialize matrices.
    for (i = 0; i < SIZE; ++i) {
      for (j = 0; j < SIZE; ++j) {
    	a[i][j] = (double)i + j;
    	b[i][j] = (double)i - j;
    	c[i][j] = 0.0f;
      }
    }
     
    // Compute matrix multiplication.
    for (i = 0; i < SIZE; ++i) {
      for (j = 0; j < SIZE; ++j) {
	#pragma acc loop independent
    	for (k = 0; k < SIZE; ++k) {
      	  c[i][j] += a[i][k] * b[k][j];
    	}
      }
    }
  } // end of kernels
  end = clock();
  printf("Execution Time: %04f sec \n", (end-begin)/CLOCKS_PER_SEC);
  return 0;
}
