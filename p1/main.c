#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <papi.h>

#define LIMIT 1000
#define LARGER 290000


//Methods for multiplying two matrices and writing to file
void mmm();
void mmm_tests();
void multiply(double **matrix1, double **matrix2, double **result, int r, int c, int p);
void free_matrix(double **matrix, int r);
double** make_matrix(size_t r, size_t col);
void set_matrix(FILE *fp, double **matrix, size_t r, size_t c);


//Methods for variant testing.
void write_to_file(double **matrix, int r, int c);
void i_j_k(double **m1, double **m2, double **result, long long counters[], int PAPI_events[]);
void i_k_j(double **m1, double **m2, double **result, long long counters[], int PAPI_events[]);
void j_i_k(double **m1, double **m2, double **result, long long counters[], int PAPI_events[]);
void j_k_i(double **m1, double **m2, double **result, long long counters[], int PAPI_events[]);
void k_i_j(double **m1, double **m2, double **result, long long counters[], int PAPI_events[]);
void k_j_i(double **m1, double **m2, double **result, long long counters[], int PAPI_events[]);
void set_matrix_random(double **matrix);
void clear_matrix(double **matrix);

void cache_print(long long counters[]);
void other_print(long long counters[]);
void clear_cache();

int SIZE;

int FLAG;

/*
Total cycles
Total instructions
Total Load Store Instructions
Total Floating Point Instructions
L1 data cache accesses and misses
L2 data cache accesses and misses
*/


int main(int argc, char const *argv[])
{
  if(argc == 1){
     mmm();
  }else if(argc == 3){
    SIZE = atoi(argv[1]);
    FLAG = atoi(argv[2]);
    mmm_tests();
  }else{
    printf("Arguments not correct\n");
    exit(0);
  }
  return 0;
}


void mmm_tests()
{
  double **m1 = make_matrix(SIZE, SIZE);
  double **m2 = make_matrix(SIZE, SIZE);
  double **result = make_matrix(SIZE, SIZE);

  int i = PAPI_library_init(PAPI_VER_CURRENT);
  int num_counters = PAPI_num_counters(); // 6


  long long counters[5];

  int PAPI_cache[] =  {
     PAPI_L1_DCM,
     PAPI_L2_DCM,
     PAPI_L1_DCA,
     PAPI_L2_DCA,
  };

 int PAPI_other[] = {
    PAPI_LD_INS,
    PAPI_SR_INS,
    PAPI_FP_INS,
    PAPI_TOT_INS,
    PAPI_TOT_CYC
 };

  set_matrix_random(m1);
  set_matrix_random(m2);

  clear_matrix(result);
  clear_cache();
  if(FLAG)
    i_j_k(m1,m2,result, counters, PAPI_cache);
  else
    i_j_k(m1,m2,result, counters, PAPI_other);


  clear_matrix(result);
  clear_cache();
  if(FLAG)
    j_i_k(m1,m2,result, counters, PAPI_cache);
  else
    j_i_k(m1,m2,result, counters, PAPI_other);


  clear_matrix(result);
  clear_cache();
  if(FLAG)
    j_k_i(m1,m2,result, counters, PAPI_cache);
  else
    j_k_i(m1,m2,result, counters, PAPI_other);

  clear_matrix(result);
  clear_cache();
  if(FLAG)
    k_j_i(m1,m2,result, counters, PAPI_cache);
  else
    k_j_i(m1,m2,result, counters, PAPI_other);

  clear_matrix(result);
  clear_cache();
  if(FLAG)
    i_k_j(m1,m2,result, counters, PAPI_cache);
  else
    i_k_j(m1,m2,result, counters, PAPI_other);

  clear_matrix(result);
  clear_cache();
  if(FLAG)
    k_i_j(m1,m2,result, counters, PAPI_cache);
  else
    k_i_j(m1,m2,result, counters, PAPI_other);



  free_matrix(m1, SIZE);
  free_matrix(m2, SIZE);
  free_matrix(result, SIZE);
}

void cache_print(long long counters[]){
  printf("L1 Data Cache miss : %lld\n", counters[0]);
  printf("L1 Data Cache accesses : %lld\n", counters[2]);
  printf("L1 Miss rate : %.2f\n", 100 * (1.0 * counters[0])/counters[2]);
  printf("L2 Data Cache miss : %lld\n", counters[1]);
  printf("L2 Data Cache accesses : %lld\n", counters[3]);
  printf("L2 Miss rate : %.2f\n", 100 * (1.0 * counters[1])/counters[3]);
}

void other_print(long long counters[]){
  printf("Load instructions: %lld\n", counters[0]);
  printf("Store instructions: %lld\n", counters[1]);
  printf("Total instructions: %lld\n", counters[3]);
  printf("Total cycle: %lld\n", counters[4]);
  printf("Total number of load and store %lld\n", counters[0] + counters[1]);
  printf("Floating point instructions: %lld\n", counters[2]);
}


void clear_cache(){
  char *to_clear = malloc(sizeof(char) * (LARGER));
  char temp = 'a';
  for(int i = 0; i < LARGER; ++i){
    temp = to_clear[i];
  }
  free(to_clear);
}


void i_j_k(double **m1, double **m2, double **result, long long counters[], int PAPI_events[])
{
  struct timespec start, end;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
  // clock_gettime(CLOCK_REALTIME, &start);
  int i = PAPI_start_counters(PAPI_events, FLAG ? 4 : 5);
  for(int i = 0; i < SIZE; ++i){
    for(int j = 0; j < SIZE; ++j){
      for(int k = 0; k < SIZE; ++k){
        result[i][j] += m1[i][k] * m2[k][j];
      }
    }
  }
  PAPI_read_counters(counters, FLAG ? 4 : 5);
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
  //clock_gettime(CLOCK_REALTIME, &end);
  printf("I J K Results\n");
  if(FLAG){
    cache_print(counters);
  }else{
    other_print(counters);
  }
  double time_taken;
  time_taken = (end.tv_sec - start.tv_sec) * 1e9;
  time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
  printf("Time taken by I J K: %f seconds \n", time_taken);
}

void i_k_j(double **m1, double **m2, double **result, long long counters[], int PAPI_events[])
{
  struct timespec start, end;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
  //clock_gettime(CLOCK_REALTIME, &start);
  int i = PAPI_start_counters(PAPI_events, FLAG ? 4 : 5);
  for(int i = 0; i < SIZE; ++i){
    for(int k = 0; k < SIZE; ++k){
      for(int j = 0; j < SIZE; ++j){
        result[i][j] += m1[i][k] * m2[k][j];
      }
    }
  }
  PAPI_read_counters(counters, FLAG ? 4 : 5);
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
 //  clock_gettime(CLOCK_REALTIME, &end);
  printf("I K J Results\n");
  if(FLAG){
    cache_print(counters);
  }else{
    other_print(counters);
  }

  double time_taken;
  time_taken = (end.tv_sec - start.tv_sec) * 1e9;
  time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
  printf("Time taken by I K J: %f seconds\n", time_taken);
}

void j_i_k(double **m1, double **m2, double **result, long long counters[], int PAPI_events[])
{
  printf("J I K Results\n");
  struct timespec start, end;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
  //clock_gettime(CLOCK_REALTIME, &start);
  int i = PAPI_start_counters(PAPI_events, FLAG ? 4 : 5);
  for(int j = 0; j < SIZE; ++j){
    for(int i = 0; i < SIZE; ++i){
      for(int k = 0; k < SIZE; ++k){
        result[i][j] += m1[i][k] * m2[k][j];
      }
    }
  }
  PAPI_read_counters(counters, FLAG ? 4 : 5);
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
  //clock_gettime(CLOCK_REALTIME, &end);
  if(FLAG){
    cache_print(counters);
  }else{
    other_print(counters);
  }

  double time_taken;
  time_taken = (end.tv_sec - start.tv_sec) * 1e9;
  time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
  printf("Time taken by J I K: %f seconds\n", time_taken);
}


void j_k_i(double **m1, double **m2, double **result, long long counters[], int PAPI_events[])
{
  struct timespec start, end;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
//  clock_gettime(CLOCK_REALTIME, &start);
  int i = PAPI_start_counters(PAPI_events, FLAG ? 4 : 5);
  for(int j = 0; j < SIZE; ++j){
    for(int k = 0; k < SIZE; ++k){
      for(int i = 0; i < SIZE; ++i){
        result[i][j] += m1[i][k] * m2[k][j];
      }
    }
  }
  PAPI_read_counters(counters, FLAG ? 4 : 5);
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
//clock_gettime(CLOCK_REALTIME, &end);
  printf("J K I Results\n");
  if(FLAG){
    cache_print(counters);
  }else{
    other_print(counters);
  }

  double time_taken;
  time_taken = (end.tv_sec - start.tv_sec) * 1e9;
  time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
  printf("Time taken by J K I: %f seconds\n", time_taken);
}

void k_i_j(double **m1, double **m2, double **result, long long counters[], int PAPI_events[])
{
  struct timespec start, end;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
//  clock_gettime(CLOCK_REALTIME, &start);
  int i = PAPI_start_counters(PAPI_events, FLAG ? 4 : 5);
  for(int k = 0; k < SIZE; ++k){
    for(int i = 0; i < SIZE; ++i){
      for(int j = 0; j < SIZE; ++j){
        result[i][j] += m1[i][k] * m2[k][j];
      }
    }
  }
  PAPI_read_counters(counters, FLAG ? 4 : 5);
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
//  clock_gettime(CLOCK_REALTIME, &end);
  printf("K I J Results\n");
  if(FLAG){
    cache_print(counters);
  }else{
    other_print(counters);
  }

  double time_taken;
  time_taken = (end.tv_sec - start.tv_sec) * 1e9;
  time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
  printf("Time taken by K I J: %f seconds\n", time_taken);
}

void k_j_i(double **m1, double **m2, double **result, long long counters[], int PAPI_events[])
{
  struct timespec start, end;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
//  clock_gettime(CLOCK_REALTIME, &start);
  int i = PAPI_start_counters(PAPI_events, FLAG ? 4 : 5);
  for(int k = 0; k < SIZE; ++k){
    for(int j = 0; j < SIZE; ++j){
      for(int i = 0; i < SIZE; ++i){
        result[i][j] += m1[i][k] * m2[k][j];
      }
    }
  }
  PAPI_read_counters(counters, FLAG ? 4 : 5);
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
//  clock_gettime(CLOCK_REALTIME, &end);
  printf("K J I Results\n");
  if(FLAG){
    cache_print(counters);
  }else{
    other_print(counters);
  }

  double time_taken;
  time_taken = (end.tv_sec - start.tv_sec) * 1e9;
  time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
  printf("Time taken by K J I: %f seconds\n", time_taken);
}

void clear_matrix(double **matrix){
  for(int i = 0; i < SIZE; ++i){
    for(int j = 0; j < SIZE; ++j){
      matrix[i][j] = 0.0;
    }
  }
}



void free_matrix(double **matrix, int r)
{
  for(int i = 0; i < r; ++i){
    free(matrix[i]);
  }
  free(matrix);
}

void set_matrix_random(double **matrix){
  for(int i = 0; i < SIZE; ++i){
    for(int j = 0; j < SIZE; ++j){
      matrix[i][j] = (rand() % LIMIT);
      //printf("%f\n",matrix[i][j]);
    }
  }
}


double** make_matrix(size_t r, size_t col)
{
  double **matrix1;
  matrix1 = malloc(sizeof(double *) * r);
  for(int i = 0; i < r; ++i){
    *(matrix1 + i) = malloc(sizeof(double) * col);
  }
  return matrix1;
}

void set_matrix(FILE *fp, double **matrix, size_t r, size_t c)
{
  for(int i = 0; i < r; ++i){
    for(int j = 0; j < c; ++j){
      int read = fscanf(fp, "%lf ", &matrix[i][j]);
      //printf("%.2lf ", matrix[i][j]);
    }
    //printf("\n");
  }
}

void multiply(double **matrix1, double **matrix2, double **result, int r, int c, int p)
{
  for(int i = 0; i < r; ++i)
  {
    for(int j = 0; j < c; ++j)
    {
      result[i][j] = 0;
      for(int k = 0; k < p; ++k)
      {
        result[i][j] += matrix1[i][k] * matrix2[k][j];
      }
      //printf("%.2lf ", result[i][j]);
    }
    //printf("\n");
  }
}

void write_to_file(double **matrix, int r, int c)
{
  FILE *fp = fopen("res.txt", "w");
  for(int i = 0; i < r; ++i){
    for(int j = 0; j < c; ++j){
      fprintf(fp, "%.2lf ", matrix[i][j]);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
}

void mmm()
{
  const char *filename = "matrix.txt";
  FILE *fp = fopen(filename, "r");
  if(!fp){
    printf("Invalid file\n");
    exit(-1);
  }else{
    int item[2];
    int read;

    read = fscanf(fp, "%d %d", item, item  + 1);
    int row_size_1 = item[0];
    int col_size_1 = item[1];
    double **matrix_1 = make_matrix(row_size_1, col_size_1);
    set_matrix(fp, matrix_1, row_size_1, col_size_1);
    read = fscanf(fp, "%d %d", item, item  + 1);
    int row_size_2 =item[0];
    int col_size_2 =item[1];

    double **matrix_2 = make_matrix(row_size_2, col_size_2);
    set_matrix(fp, matrix_2, row_size_2, col_size_2);
    double **result = make_matrix(row_size_1, col_size_2);

    multiply(matrix_1, matrix_2, result, row_size_1, col_size_2, col_size_1);
    write_to_file(result, row_size_1, col_size_2);


    free_matrix(matrix_1, row_size_1);
    free_matrix(matrix_2, row_size_2);
    free_matrix(result, row_size_1);
  }
  fclose(fp);
}
