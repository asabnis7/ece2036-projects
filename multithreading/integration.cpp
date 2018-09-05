// Parallel Numerical Integration
// ARJUN SABNIS

#include <iostream>
#include <vector>
#include <array>
#include <math.h>
#include <time.h>
#include "gthread.h"

using namespace std;

vector<double> poly_coeffs;
array<double, 10> partial_sums = {};

double f(double x)
{	
	double sum = 0;
	for (int n = 0; n < poly_coeffs.size(); n++)
	{
		sum += poly_coeffs[n] * pow(x, n);
	}
	return sum;
}

double analytic_integral(double lb, double ub)
{
	double sum;
	for (int n = 0; n < poly_coeffs.size(); n++)
	{
		sum += ((poly_coeffs[n] * pow(ub, n + 1)) / (n + 1)) - ((poly_coeffs[n] * pow(lb, n + 1)) / (n + 1));
	}
	return sum;
}

double numerical_integral(double lb, double ub, int gp)
{
	double sum;
	double dx = ub / gp;
	for (int i = 0; i < gp; i++)
	{
		double xi = lb + (i*dx);
		sum += (f(xi)*dx);
	}
	return sum;
}
  
void threaded_numerical_integral(double lb, double ub, int gp, int thread_num)
{
	double thread_ub = ub - lb;
	partial_sums[thread_num] = numerical_integral(lb, thread_ub, gp);
	EndThread();
}


int main()
{
  time_t start_time;
  time_t end_time;
  double computation_time;
  const int N_threads = 10;
  double x_min = 0;
  double x_max = 100;
  double x_min_m, x_max_m, relative_error;
  const int N_points1 = 1000;
  const int N_points2 = 100000000;

  double temp_coeff, x;
  double analytic_result, num_result1, num_result2, num_result2_threaded;
  double rel_error1, rel_error2, rel_error2_threaded;

  cout << "Welcome to the numerical integrator." << endl;

  cout << "Please enter the coefficient of the 0th-degree term of your polynomial: ";
  int n = 0;

  while (cin >> temp_coeff)
  {
	  poly_coeffs.push_back(temp_coeff);
	  n++;
	  cout << "Please enter the coefficient of the term of degree " << n << ", or 'x' to stop: ";
  }

  cout << "The coefficients you entered were: ";
  for (int i = 0; i < poly_coeffs.size(); i++)
  {
	  cout << poly_coeffs[i] << "  ";
  }
  cout << endl;

  analytic_result = analytic_integral(x_min, x_max);
  cout << "The analytically calculated integral of your polynomial is: " << analytic_result << endl; 

  num_result1 = numerical_integral(x_min, x_max, N_points1);
  rel_error1 = (analytic_result - num_result1) / analytic_result;
  cout << "The relative error of the numerical integration with " << N_points1 << " points is: " << rel_error1 << endl; 
     
  time(&start_time);
  num_result2 = numerical_integral(x_min, x_max, N_points2);
  time(&end_time);
  computation_time = difftime(end_time, start_time); 
  rel_error2 = (analytic_result - num_result2) / analytic_result;
  cout << "The relative error of the numerical integration with " << N_points2 << " points is: " << rel_error2 << endl; 
  cout << "The computation time with one thread was: " << computation_time << " seconds." << endl;

  // begin thread creation
  time(&start_time);
  for (int thread = 0; thread < N_threads; thread++)
  {
	  CreateThread(&threaded_numerical_integral, x_min + (10 * thread), x_min + (10 * (thread + 1)), N_points2 / N_threads, thread);
  }
  WaitAllThreads();
  time(&end_time);
  
  // sum all partial sums
  for (int i = 0; i < partial_sums.size(); i++)
  {
	  num_result2_threaded += partial_sums[i];
  }
  
  computation_time = difftime(end_time, start_time);
  rel_error2_threaded = (analytic_result - num_result2_threaded) / analytic_result;
  cout << "The relative error of the numerical integration with " << N_points2 << " points and " << N_threads <<  " threads is: " << rel_error2_threaded << endl;
  cout << "The computation time with ten threads was: " << computation_time << " seconds." << endl;
}
