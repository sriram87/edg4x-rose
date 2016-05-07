#include "_mpi.h"

int CompDebugAssert(int expr);

int main(int argc, char* argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int x, y;
  MPI_Status status;
  if(rank == 0) {
    x = 2;
    MPI_Send(&x, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(&y, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &status);
  }
  else if(rank == 1) {
    MPI_Recv(&x, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
    y=x+1;
    MPI_Send(&y, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
  }

  CompDebugAssert(y==3);

  MPI_Finalize();
  return 0;
}
