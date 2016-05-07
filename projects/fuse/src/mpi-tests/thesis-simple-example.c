#include "_mpi.h"

#pragma fuse seq(mcc, pt, cp, mv, cp, dp, mdv, mco, mdv)

int main() {
  int x, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank == 0) {
    x = 5;
    MPI_Send(&x, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
  }
  else if(rank == 1) {
    MPI_Recv(&x, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  return 0;
}
