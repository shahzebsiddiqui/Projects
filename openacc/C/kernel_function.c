#include "stdio.h"
#include "openacc.h"
#include "math.h"
#include "stdlib.h"
#include "time.h"

void init(double * restrict v, int n, double value)
{
  printf("Kernel: init\n");
#pragma acc kernels present(v)
  for (int j = 0; j < n; j++)
    {
      v[j] = j*j - 500*j + 45*log(j+1);
    }
}
void getMax(double *restrict v, int n, double seed)
{
  double maxValue = -1.0;
#pragma acc parallel reduction(max:maxValue) present(v)
  {
   for (int j = 0; j < n; j++)
    {
      if (maxValue < v[j])
	{
	  maxValue = v[j];    
	}
    }
  }
  printf("Max = %f\n",maxValue);
}
void init_dev(double * restrict v, int n, double seed)
{
  printf("Kernel: init_dev\n");
#pragma acc kernels deviceptr(v)
  {
    for(int j = 0; j < n; j++)
      {
	v[j] = j*j - 500*j + 45*log(j+1);      
      }
  }  
}
void getMax_dev(double *restrict v, int n, double seed)
{
  double maxValue = -1.0;
#pragma acc parallel deviceptr(v) reduction(max:maxValue)
  {
   for (int j = 0; j < n; j++)
    {
      if (maxValue < v[j])
	{
	  maxValue = v[j];    
	}
    }
  }
  printf("Max = %f \n",maxValue);
}
int main()
{
  int n = 500000;
  int i;
  double * a;
  acc_set_device_num(0,acc_device_nvidia);
  acc_init(acc_device_nvidia);
  a = (double*)malloc(sizeof(double)*n);
  srand((unsigned int)time(NULL));

  double seed = (double)rand()/(double)(RAND_MAX);
  printf("Seed = %f\n",seed);
  #pragma acc data copy(a[0:n])
  {
    init(a,n,seed);
    getMax(a,n,seed);
  }
  

  double * restrict dev_a;
  dev_a = (double * restrict) acc_malloc(sizeof(double)*n);
 
  
  init_dev(dev_a,n,seed);
  getMax_dev(dev_a,n,seed);
  acc_free(dev_a);

  return 0;
}
