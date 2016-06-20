#include "_mpi.h"

#pragma fuse-mpi-tc seq(mcc, pt, cp, mv, cp, dp, mco, cp, dp, cp)

int CompDebugAssert(int expr);

int main(int argc, char* argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int x, y, z;
  int t = 0;

  while(t < 100) {
    if(rank == 0) {
      x = 2;
      MPI_Send(&x, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
      z = 3;
    }
    else if(rank == 1) {
      MPI_Status status;    
      MPI_Recv(&y, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
      if(y == 2) z = 3;
    }
    ++t;
  }

  CompDebugAssert(z == 3);

  MPI_Finalize();
  return 0;
}