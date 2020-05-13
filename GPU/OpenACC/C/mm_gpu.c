#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <openacc.h>
int main(int argc, char* argv[])
{
  int i,j,k;
  int N = 10000;
  double ** restrict X,** restrict Y,** restrict Z;
  
  X = malloc(N*sizeof(double*)*N);
  Y = malloc(N*sizeof(double*)*N);
  Z = malloc(N*sizeof(double*)*N);
  
  for (i = 0; i < N; i++)
  {
	X[i] = malloc(sizeof(double)*N);
	Y[i] = malloc(sizeof(double)*N);
	Z[i] = malloc(sizeof(double)*N);
  }
  #pragma acc declare create(X[0:N][0:N],Y[0:N][0:N],Z[0:N][0:N])
  for (i = 0; i < N; i++)
  {
	for (j = 0; j < N; j++)
	{
		X[i][j] = sin(i)*cos(j);
		Y[i][j] = log(1+i*j);
	}
  }
  #pragma acc kernels present(X,Y,Z)
  for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
        {
          Z[i][j] = 0;
 	  #pragma acc loop vector
          for (k = 0; k < N; k++)
            {
              Z[i][j] += X[i][k]*Y[k][j];
            }
        }
    }

return 0;


}
