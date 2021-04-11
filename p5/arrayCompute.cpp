#include <pthread.h> /*used in other parts of the assignment */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>  /* for uint64  */
#include <time.h>    /* for clock_gettime */
#include <atomic>    /*used in other parts of the assignment */\
#include <vector>


#define MAX_THREADS 512

int threadArg[MAX_THREADS];
pthread_t handles[MAX_THREADS];
double part[MAX_THREADS];

const int numPoints = 1000000000;

const double step = 0.5 / numPoints;

int num_threads;

double f(double x) {
  return (6.0/sqrt(1-x*x));
}

void *performComputation(void *threadIdPtr) {
  int threadid = *(int*)threadIdPtr;
  printf("Thread %d starting\n", threadid);
  double total = 0;
  for(int i = threadid; i < numPoints; i+=num_threads) {
    double val =  step * ((double) i);
    double to_add = step*f(val);
    total += to_add;
  }
  part[threadid] = total;
  printf("Thread %d finished\n", threadid);
}



int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("Please enter the number of threads\n");
    exit(1);
  } else {
    num_threads = atoi(argv[1]);
  }

  uint64_t execTime; /*time in nanoseconds */
  struct timespec tick, tock;

  vector<pthread_t*> workers;
  barrier task_barrier(num_threads);

  pthread_attr_t attr;
  pthread_attr_init (&attr);

  clock_gettime(CLOCK_MONOTONIC_RAW, &tick);

  int start = 0;
  for(int t= 0; t < num_threads; ++t) {
    threadArg[t] = t;
    pthread_create(&handles[t],&attr,performComputation,& threadArg[t]);
  }

  for(int t = 0; t < num_threads; ++t) {
    pthread_join(handles[t], NULL);
  }

  clock_gettime(CLOCK_MONOTONIC_RAW, &tock);

  double pi = 0.0f;

  for(int i = 0; i < num_threads; ++i) {
    pi += part[i];
  }

  execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;

  printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);

  printf("%.20f\n", pi);

  pthread_exit(NULL);
  return 0;
}
