#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mmm();
void mmm_tests(size_t n);
void multiply(double **matrix1, double **matrix2, double **result, int r, int c, int p);
void free_matrix(double **matrix, int r);
double** make_matrix(size_t r, size_t col);
void set_matrix(FILE *fp, double **matrix, size_t r, size_t c);
void write_to_file(double **matrix, int r, int c);



int main(int argc, char const *argv[]) {

  if(argc == 1){
     mmm();
  }else {
    int n = atoi(argv[1]);
    mmm_tests(n);
  }
  return 0;
}


void mmm_tests(size_t n){
  
}

void free_matrix(double **matrix, int r)
{
  for(int i = 0; i < r; ++i){
    free(matrix[i]);
  }
  free(matrix);
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
      fscanf(fp, "%lf ", &matrix[i][j]);
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

void write_to_file(double **matrix, int r, int c){
  FILE *fp = fopen("res.txt", "w");
  for(int i = 0; i < r; ++i){
    for(int j = 0; j < c; ++j){
      fprintf(fp, "%.1lf ", matrix[i][j]);
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
    fscanf(fp, "%d %d", item, item  + 1);
    int row_size_1 = item[0];
    int col_size_1 = item[1];
    double **matrix_1 = make_matrix(row_size_1, col_size_1);
    set_matrix(fp, matrix_1, row_size_1, col_size_1);
    fscanf(fp, "%d %d", item, item  + 1);
    int row_size_2 =item[0];
    int col_size_2 =item[1];
    double **matrix_2 = make_matrix(row_size_2, col_size_2);
    set_matrix(fp, matrix_2, row_size_2, col_size_2);
    double **result = make_matrix(row_size_1, col_size_2);
    multiply(matrix_1, matrix_2, result, row_size_1, col_size_2, col_size_1);
    write_to_file(result, row_size_1, col_size_2);
  }
  fclose(fp);
}
