#include <pthread.h> /*used in other parts of the assignment */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>  /* for uint64  */
#include <time.h>    /* for clock_gettime */
#include <atomic>    /*used in other parts of the assignment */

#define NUM_THREADS 8

int threadArg[NUM_THREADS];
pthread_t handles[NUM_THREADS];
int start_points[NUM_THREADS];

double part_computation[NUM_THREADS];

double pi = 0.0;

const int numPoints = 1000000000;

const double step = 0.5/numPoints;

const int num_per_thread = numPoints / NUM_THREADS;

double f(double x) {
  return (6.0/sqrt(1-x*x));
}



void *performComputation(void *start_location) {
  int start = *(int*)(start_location);
  int end = start + num_per_thread;
  printf("start: %d end: %d\n", start, end);
  double x_curr = 0.0d + (step * start);
  double total = 0;
  for(int count = start; count < end; ++count) {
    total = total + step * f(x_curr);
    x_curr = x_curr + step;
  }
  printf("From starting point %d the total is %.20f\n", start, total);
  part_computation[start/num_per_thread] = total;
}



int main(int argc, char *argv[]) {
  uint64_t execTime; /*time in nanoseconds */
  struct timespec tick, tock;

  clock_gettime(CLOCK_MONOTONIC_RAW, &tick);

  // double x = 0.0d;
  // for (int i = 0; i < numPoints; i++) {
  //  pi = pi + step*f(x);  // Add to local sum
  //  x = x + step;  // next x
  // }


  int start = 0;
  for(int t= 0; t < NUM_THREADS; ++t) {
    threadArg[t] = t;
    start_points[t] = start;
    pthread_create(&handles[t], NULL, performComputation, &start_points[t]);
    start += num_per_thread;
  }


  for(int t = 0; t < NUM_THREADS; ++t) {
    pthread_join(handles[t], NULL);
  }


  clock_gettime(CLOCK_MONOTONIC_RAW, &tock);

  execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;

  for(int i = 0; i < NUM_THREADS; ++i) {
    pi += part_computation[i];
  }

  printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);

  printf("%.20f\n", pi);

  pthread_exit(NULL);
  return 0;
}
