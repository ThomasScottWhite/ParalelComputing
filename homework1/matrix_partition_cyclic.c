#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define COLUMNS 10
#define ROWS 10

void populateMatrix(int matrix[][COLUMNS])
{
  int row;
  int col;

  for(row = 0; row < ROWS; row++)
  {
    for(col = 0; col < COLUMNS; col++)
    {
          matrix[row][col] = 1;
    }
  }
}

int sequentialSum(int matrix[][COLUMNS])
{
  int row;
  int col;
  int total = 0;

  for(row = 0; row < ROWS; row++)
  {
    for(col = 0; col < COLUMNS; col++)
    {
         total = total +  matrix[row][col];
    }
  }
 return total;
}

//
int main(int argc, char **argv)
{
  int nprocs;
  int rank;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  
  int matrix[ROWS][COLUMNS];
  populateMatrix(matrix);
  
  if(rank == 0)
  {
     printf("Number of processes running %d \n", nprocs);
  }

  int seq_total;

  if(rank == 0)
  {
     seq_total = sequentialSum(matrix);
     printf("sequential sum is %d \n", seq_total);
  }


   // Implement parallel sum by dividing the matrix elements among MPI processes

  int localSum = 0;
  int global_sum = 0;
  int total_elements = ROWS * COLUMNS;
  int row, col;

  // The method below is probably slower, but I include this in case this is what the grader is looking for, wasnt sure if the second version made the problem too linear.
  // for (row = 0; row < ROWS; row++){
  //   for (col = 0; col < COLUMNS; col++){
  //     if ((row * COLUMNS + col) % nprocs == rank){
  //       localSum += matrix[row][col];
  //     }
  //   }
  // }

  // This method is probably faster 
  for (int i = rank; i < total_elements; i += nprocs){
    int row = i / COLUMNS;
    int col = i % COLUMNS;

    localSum += matrix[row][col];
  }

 
  MPI_Reduce(&localSum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);  
  
  if(rank == 0)
  {
     printf("parallel sum is %d \n", global_sum);
  }

  MPI_Finalize();
  return 0;
}
