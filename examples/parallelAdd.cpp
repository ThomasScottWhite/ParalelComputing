#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define N 101

// compile: mpicxx -o sum parallelAdd.cpp
// run: mpirun -np 4 ./sum

int main(int argc, char **argv)
{
  int nprocs;
  int rank;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  
   /* Start timer */
   MPI_Barrier (MPI_COMM_WORLD);
   
   double elapsed_time;  /* Time to find, count solutions */
   elapsed_time = - MPI_Wtime();
  
  int a[N];
  
  int i;
  for(i = 0; i < N; i++)
  {
      a[i] = 1;
  }
  
  int startIndex = (rank * N)/nprocs;
  int endIndex = ((rank+1) * N)/nprocs;  
  
  if(rank == (nprocs - 1))
  {
      endIndex = N;
  }
  
  int localSum = 0;
  for(i = startIndex; i < endIndex; i++ )
  {
     localSum += a[i];
  }
  
  
  //printf("endIndex by rank = %d is %d \n", rank, endIndex);
  printf("local Sum by rank = %d is %d \n", rank, localSum);
  fflush(stdout);
  
  int globalSum = 0;
   
  // MPI reduce is to add the local sums to global sum; it is a communication function
  // The final globalSum should be visible only for process 0.
  MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);  
  
   /* Stop timer */
   elapsed_time += MPI_Wtime();

  
   if(rank == 0)
   {
      printf("global sum is %d \n", globalSum);
      printf ("Execution time %8.6f\n", elapsed_time);
      fflush(stdout);
   }

   MPI_Finalize();
   return 0;
}