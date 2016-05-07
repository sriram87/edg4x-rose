#include "_mpi.h"

#pragma fuse seq(mcc, cp, pt, mv, cp, dp)

int CompDebugAssert(int expr);

int main(int argc, char* argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int x, y;
  int t = 0;
  while(t < 100) {
    if(rank == 0) {
      x = 2;
      MPI_Send(&x, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
      y = 3;
      MPI_Send(&y, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
    }
    else if(rank == 1) {
      MPI_Status status;
      MPI_Recv(&x, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(&y, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
    }
    t++;
  }
  CompDebugAssert(x==2);
  //  CompDebugAssert(y==3);
  MPI_Finalize();
  return 0;
}
