#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

// compile: mpicxx -o hello hellompi.cpp
// module load mpi
// run:     mpirun -np 3 ./hello

int main(int argc, char **argv)
{
  int rank; 
  int numProcesses;
  
  char hostname[256];

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
  
  gethostname(hostname,255);

  printf("Hello world!  I am process number: %d on host %s total %d \n", rank, hostname, numProcesses);

  MPI_Finalize();

  return 0;
}