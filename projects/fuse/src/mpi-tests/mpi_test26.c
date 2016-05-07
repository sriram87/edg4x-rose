#include "_mpi.h"

#pragma fuse-mpi-seq seq(mcc, pt, cp, mv, cp, dp, mco, cp)

int CompDebugAssert(int expr);

int main(int argc, char* argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int x, y;
  int t = 0;
  while(t < 100) {
    if(rank % 2 == 0) {
      x = 2;
    }
    else  {
      x = 4;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    t++;
  }
  CompDebugAssert(x==2);

  MPI_Finalize();
  return 0;
}
