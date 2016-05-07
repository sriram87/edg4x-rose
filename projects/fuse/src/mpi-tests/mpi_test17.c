#include "_mpi.h"

int CompDebugAssert(int expr);

#pragma fuse seq(mcc, pt, cp, mv, cp, dp, mdv, mco, mdv)

int main(int argc, char* argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int msg;
  msg = 2;
  int t=0;
  while(t < 100) {
    if(rank == 0) {
      MPI_Send(&msg, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
      msg++;
    }
    else if(rank == 1) {
      MPI_Status status;
      MPI_Recv(&msg, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
      msg = msg + 1;
    }
    t++;
  }

  CompDebugAssert(msg == 3);

  MPI_Finalize();
  return 0;
}
