#include "_mpi.h"

void CompDebugAssert(int);

int main() {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if(rank == 0) {
    CompDebugAssert(rank == 0 && size == 2);
  }
  else if(rank == 1) {
    CompDebugAssert(rank == 1 && size == 2);
  }
  return 0;
}
