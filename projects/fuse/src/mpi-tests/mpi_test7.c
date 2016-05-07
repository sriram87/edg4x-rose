#include "_mpi.h"

int CompDebugAssert(int expr);

#pragma fuse seq(mcc, cp, pt, mv, cp, dp, mdv, mco, mdv)

int main(int argc, char* argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int x, y;
  if(rank == 0) {
    MPI_Status status;
    x = 2;
    MPI_Recv(&x, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &status);
    y = 3;
    MPI_Recv(&y, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &status);
  }
  else if(rank == 1) {
    MPI_Status status;
    x=2;
    MPI_Send(&x, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
    y=3;
    MPI_Send(&y, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
  }

  CompDebugAssert(y == 3);

  MPI_Finalize();
  return 0;
}
