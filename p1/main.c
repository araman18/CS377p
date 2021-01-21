#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define LIMIT 100
//i-j-k, j-i-k, j-k-i, k-j-i, i-k-j, k-i-j
void initialize(double matrix_1[][SIZE], double matrix_2[][SIZE]);
void test_i_j_k(double matrix_1[][SIZE], double matrix_2[][SIZE], double result[][SIZE]);
void test_j_i_k(double matrix_1[][SIZE], double matrix_2[][SIZE], double result[][SIZE]);
void test_j_k_i(double matrix_1[][SIZE], double matrix_2[][SIZE], double result[][SIZE]);
void test_k_j_i(double matrix_1[][SIZE], double matrix_2[][SIZE], double result[][SIZE]);
void test_i_k_j(double matrix_1[][SIZE], double matrix_2[][SIZE], double result[][SIZE]);
void test_k_i_j(double matrix_1[][SIZE], double matrix_2[][SIZE], double result[][SIZE]);



int main(int argc, char const *argv[])
{
  /* code */
  double matrix_1[SIZE][SIZE] = {0};
  double matrix_2[SIZE][SIZE] = {0};
  initialize(matrix_1, matrix_2);
  double result[SIZE][SIZE] = {0};
  

  return 0;
}


void test_i_j_k(double matrix_1[][SIZE], double matrix_2[][SIZE], double result[][SIZE])
{
  for(int i = 0; i < SIZE; ++i){
    for(int j = 0; j < SIZE; ++j){
      for(int k = 0; k < SIZE: ++k){
        result[i][j] += (matrix_1[i][k] * matrix_2[k][j]);
      }
    }
  }
}


void initialize(double matrix_1[][SIZE], double matrix_2[][SIZE])
{
  srand(time(NULL));
  for(int r = 0; r < SIZE; ++r){
    for(int c = 0; c < SIZE; ++c){
      matrix_1[r][c] = (rand() % (LIMIT));
      matrix_2[r][c] = (rand() % (LIMIT));
      //printf("%f %f\n", matrix_1[r][c], matrix_2[r][c]);
    }
  }
}
