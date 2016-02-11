#include <thrust/host_vector.h>
#include <thrust/random.h>
#include <thrust/sort.h>
#include <thrust/copy.h>
#include <thrust/reduce.h>
using namespace std;

struct is_A{
  __host__ __device__
  bool operator()(const float x)
  {
	return (x >= 90);
  }
};
struct is_B{
  __host__ __device__
  bool operator()(const float x)
  {
        return (x >= 80 && x < 90);
  }
};
struct is_C{
  __host__ __device__
  bool operator()(const float x)
  {
        return (x >= 70 && x < 80);
  }
};
struct is_D{
  __host__ __device__
  bool operator()(const float x)
  {
        return (x >= 60 && x < 70);
  }
};
struct is_F{
  __host__ __device__
  bool operator()(const float x)
  {
        return (x < 60);
  }
};

int main(void)
{
  int N = 50;
  thrust::host_vector<float> grades(N);
  thrust::host_vector<float> A_grades(N);
  thrust::default_random_engine rng;
  thrust::uniform_int_distribution<float> dist(50,100);
  for (int i = 0; i < N; i++)
  {
    grades[i] = dist(rng);
  }
 
  
  thrust::sort(grades.begin(),grades.end()); 
  thrust::copy(grades.begin(), grades.end(), std::ostream_iterator<float>(std::cout, "\n"));
  
  int countA = thrust::count_if(grades.begin(),grades.end(),is_A());
  int countB = thrust::count_if(grades.begin(),grades.end(),is_B());
  int countC = thrust::count_if(grades.begin(),grades.end(),is_C());
  int countD = thrust::count_if(grades.begin(),grades.end(),is_D());
  int countF = thrust::count_if(grades.begin(),grades.end(),is_F());

  float sum = thrust::reduce(grades.begin(),grades.end());
  float mean = sum / N;
  cout << "mean: " <<  mean << "\n";
  cout << "Grade Distribution by Letter\n";
  cout << "A: " << countA << "\n";
  cout << "B: " << countB << "\n";
  cout << "C: " << countC << "\n";
  cout << "D: " << countD << "\n";
  cout << "F: " << countF << "\n";
  return 0;
}
