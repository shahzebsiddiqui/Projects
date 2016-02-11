#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/copy.h>
#include <thrust/transform.h>
#include <thrust/fill.h>
#include <iostream>

using namespace std;

int main(void)
{
  int N = 24;
  thrust::host_vector<int> H1(N);
  thrust::host_vector<int> H2(N);
  thrust::host_vector<int> H3(N);
  for (int i = 0; i < N; i++)
  { 
    H1[i] = 2*i;
    H2[i] = 3*i;
    //cout << "H[" << i << "]:" << H[i] << endl;
  }

  thrust::device_vector<int> D1 = H1;
  thrust::device_vector<int> D2 = H2;
  thrust::device_vector<int> D3(N);
  for (int i = 0; i < N; i++)
    D3[i] = D1[i] + D2[i];
  
  H3 = D3;
  cout << "H1 \t + \tH2 \t = \t H3" << endl;
  for (int i = 0; i < N; i++)
    cout << H1[i] << "\t + \t" << H2[i] << "\t = \t" <<  H3[i] << endl;

  // set D3 to 0
  thrust::fill(D3.begin(), D3.end(),0);
  // D3 = D1 + D2
  thrust::transform(D1.begin(), D1.end(), D2.begin(), D3.begin(), thrust::plus<float>());
  // copy D3 array to stdout for print
  thrust::copy(D3.begin(),D3.end(),ostream_iterator<int>(cout,"\n"));
  return 0;
}
