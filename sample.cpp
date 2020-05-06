#include <iostream>
#include <string>

#include <mpi.h>

void err(const int result, const int rank, const char *message)
{
  std::cerr << "[Rank " << rank << "] Error " << result << ": " << message << std::endl;
}

int main(int ac, char **av)
{
  int result;
  int rank = -1;

  result = MPI_Init(&ac, &av);
  if (result)
  {
    err(result, rank, "MPI_Init");
    return result;
  }

  result = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (result)
  {
    err(result, rank, "MPI_Comm_rank");
    return result;
  }

  int data = rank;
  int count = 1;
  MPI_Datatype datatype = MPI_INT;
  int dest = rank + 1;
  int tag = 0;

  std::string action;
  int arg;
  if (rank % 2)
  {
    result = MPI_Recv(&data, count, datatype, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (result)
    {
      err(result, rank, "MPI_Recv");
      return result;
    }
    action = "recv";
    arg = data;
  }
  else
  {
    result = MPI_Send(&data, count, datatype, dest, tag, MPI_COMM_WORLD);
    if (result)
    {
      err(result, rank, "MPI_Send");
      return result;
    }
    action = "send";
    arg = dest;
  }
  std::cout << rank << " " << action << " " << arg << std::endl;

  result = MPI_Finalize();
  if (result)
  {
    err(result, rank, "MPI_Finalize");
    return result;
  }

  return 0;
}
