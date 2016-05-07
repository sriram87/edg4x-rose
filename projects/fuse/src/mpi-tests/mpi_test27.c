#include "_mpi.h"

//#pragma fuse-mpi-seq seq(mcc, pt, cp, mv, cp, dp, mco, cp)

#pragma fuse-mpi-seq seq(cp)

int CompDebugAssert(int expr);

int main(int argc, char* argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();
  return 0;
}
