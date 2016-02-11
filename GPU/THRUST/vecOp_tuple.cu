#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform.h>
#include <stdlib.h>

const float alpha = 0.375;
const float beta = 0.875;
struct vectorScaleAdd
{
  template <typename Tuple>
  __host__ __device__
  void operator()(Tuple t)
  {
    thrust::get<2>(t) = alpha*thrust::get<0>(t) + beta*thrust::get<1>(t);
  }
};

int main(void)
{
  int N = 4;
//  float alpha = 0.5, beta = 0.25;

  thrust::host_vector<float> hA(N);
  thrust::host_vector<float> hB(N);
  thrust::host_vector<float> hC(N);
  
  thrust::device_vector<float> dA(N);
  thrust::device_vector<float> dB(N);
  thrust::device_vector<float> dC(N);
  
  for (int i = 0; i < N; i++)
  {
    dA[i] = (rand() % 256)/256.0;
    dB[i] = (rand() % 256)/256.0;
  }
  
  
//  thrust::transform(dA.begin(),dA.end(),dB.begin(),dC.begin(),thrust::plus<float>());
  thrust::for_each(thrust::make_zip_iterator(thrust::make_tuple(dA.begin(),dB.begin(),dC.begin())),
		   thrust::make_zip_iterator(thrust::make_tuple(dA.end(),dB.end(),dC.end())),
		   vectorScaleAdd());
		
  hA = dA;
  hB = dB;
  hC = dC;
  printf(" alpha * H1         +          beta * H2          =                    H3\n");  
  for (int i = 0; i < N; i++)
  {
    printf("%.3f * %.3f         +       %.3f * %.3f         =            %.3f\n", alpha, hA[i], beta, hB[i],hC[i]);
  }
  return 0;
}
