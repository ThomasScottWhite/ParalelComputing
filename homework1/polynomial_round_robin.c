#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define MAX 100000
#define COEFFICIENT 1


double power(double x, int degree)
{     
      if(degree == 0)  return 1;
      
      if(degree == 1)  return x;

      return x * power(x, degree - 1);
}

double sequential(int coeffArr[], double x)
{
   int maxDegree = MAX - 1;
   int i;
   double  answer = 0;
   
   for( i = 0; i < maxDegree;  i++)
   {
      
      double powerX = power(x, i);

      //printf("%f ", powerX);
      answer = answer + coeffArr[i] * powerX;
   }
   return answer;
}

void initialize(int coeffArr[])
{
   int maxDegree = MAX - 1;
   int i;
   for( i = 0; i < maxDegree; i++)
   {
      coeffArr[i] = COEFFICIENT;
   }
}

// Driver Code
int main(int argc, char **argv)
{
    int *coeffArr = (int *)malloc(sizeof(int) * MAX);
    
    initialize(coeffArr);
    double x = 0.99;
    
    int nprocs;
    int rank;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

   if(rank == 0)
   {
      printf("Number of processes running %d \n", nprocs);
   }
    /* Start timer */
    double elapsed_time = - MPI_Wtime();
   double localSum = 0.0;

   for (int i = rank; i < MAX; i += nprocs){
      double powerX = power(x, i);
      localSum += coeffArr[i] * powerX;

   }
   double global_sum = 0.0;
   MPI_Reduce(&localSum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    /* End timer */
    elapsed_time = elapsed_time + MPI_Wtime();
        
    if (rank == 0) {
      printf(" paralel value %f wall clock time %8.6f \n", global_sum, elapsed_time); 
      fflush(stdout);
   }
    
    free(coeffArr);
    MPI_Finalize();
    
	return 0;
}
