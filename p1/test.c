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
void matrix_equal(double m1[][SIZE], double m2[][SIZE]);
void perform_tests();

/*
i j k
0 0 0
0 0 1
0 1 0
0 1 1
1 0 0
1 0 1
1 1 0
1 1 1
Correct
Test j i k

1 0 0
1 0 1
0 1 0
0 1 1
1 1 0
1 1 1

*/



int main(int argc, char const *argv[])
{
  /* code */

  return 0;
}


void mmm(){

}


void perform_tests(){
  double matrix_1[SIZE][SIZE] = {0};
  double matrix_2[SIZE][SIZE] = {0};
  initialize(matrix_1, matrix_2);
  double real_result[SIZE][SIZE] = {0};
  test_i_j_k(matrix_1, matrix_2, real_result);
  double test[SIZE][SIZE] = {0};

  // TESTING IF ALGORITHMS WORK
  printf("Test i j k\n");
  test_i_j_k(matrix_1, matrix_2, test);
  matrix_equal(real_result, test);
  printf("Test j i k\n");
  test_j_i_k(matrix_1, matrix_2, test);
  matrix_equal(real_result, test);
}



void test_j_k_i(double matrix_1[][SIZE], double matrix_2[][SIZE], double result[][SIZE]){
  for(int j = 0; j < SIZE; ++j){
    for(int i = 0; i < SIZE; ++i){
      result[j][i] = 0;
      for(int k = 0; k < SIZE; ++k){
        result[i][j] += (matrix_1[j][k] * matrix_2[k][i]);
      }
    }
  }
}

//CORRECT
void test_j_i_k(double matrix_1[][SIZE], double matrix_2[][SIZE], double result[][SIZE]){
  for(int j = 0; j < SIZE; ++j){
    for(int i = 0; i < SIZE; ++i){
      result[j][i] = 0;
      for(int k = 0; k < SIZE; ++k){
        result[j][i] += (matrix_1[j][k] * matrix_2[k][i]);
      }
    }
  }
}

//CORRECT
void test_i_j_k(double matrix_1[][SIZE], double matrix_2[][SIZE], double result[][SIZE])
{
  for(int i = 0; i < SIZE; ++i){
    for(int j = 0; j < SIZE; ++j){
      for(int k = 0; k < SIZE; ++k){
        result[i][j] += (matrix_1[i][k] * matrix_2[k][j]);
      }
    }
  }
}


void test_k_j_i(double matrix_1[][SIZE], double matrix_2[][SIZE], double result[][SIZE])
{

}

void test_i_k_j(double matrix_1[][SIZE], double matrix_2[][SIZE], double result[][SIZE])
{

}

void test_k_i_j(double matrix_1[][SIZE], double matrix_2[][SIZE], double result[][SIZE])
{

}



void matrix_equal(double m1[][SIZE], double m2[][SIZE]){
  for(int i = 0; i < SIZE; ++i){
    for(int j =0 ; j < SIZE; ++j){
       //printf("%f %f\n", m1[i][j], m2[i][j]);
      if(m1[i][j] != m2[i][j]){
        printf("Wrong\n");
        return;
      }
    }
  }
  printf("Correct\n");
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
