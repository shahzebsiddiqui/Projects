#include <stdio.h>
#include <time.h>
#define SIZE 3000
float a[SIZE][SIZE];
float b[SIZE][SIZE];
float c[SIZE][SIZE];
 
int main()
{
  int i,j,k;
  double begin, end;

   begin = clock();
   // Initialize matrices.
   for (i = 0; i < SIZE; ++i) {
      for (j = 0; j < SIZE; ++j) {
    	a[i][j] = (float)i + j;
	b[i][j] = (float)i - j;
	c[i][j] = 0.0f;
      }
    }
     
    // Compute matrix multiplication.
    for (i = 0; i < SIZE; ++i) {
      for (j = 0; j < SIZE; ++j) {
        for (k = 0; k < SIZE; ++k) {
      	  c[i][j] += a[i][k] * b[k][j];
    	}
      }
    }
    end = clock();
    printf("Execution Time: %04f sec\n", (end-begin)/CLOCKS_PER_SEC);
 
  return 0;
}
