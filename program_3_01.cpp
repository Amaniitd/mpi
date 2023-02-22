
#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;

int main(void) {
   char greeting[MAX_STRING];
   int comm_sz;
   int my_rank;

   MPI_Init(NULL, NULL);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   if (my_rank != 0) {
      sprintf(greeting, "Greetings from process %d of %d!", my_rank, comm_sz);
      MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0,
            MPI_COMM_WORLD);
   } else {
      printf("Greetings from process %d of %d!\n", my_rank, comm_sz);
      for (int q = 1; q < comm_sz; q++) {
         MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         printf("%s\n", greeting);
      }
   }
   MPI_Finalize();
   return 0;
}

// compile: mpicc -g -Wall -o program_3_01 program_3_01.cpp
// run: mpiexec -n <number of processes> ./program_3_01

/*
------- Syntax -----------

------ 3.1.3 -------
// No mpi call before this
int MPI_Init( // start
   int* argc_p,
   int* argv_p
);

int MPI_Finalize(void); // end
// No mpi call after this

------ 3.1.4 ------ Communicators
int MPI_Comm_size(
   MPI_Comm comm,  // (in) communicator name (MPI_COMM_WORLD)
   int* size_p   // (out) number of processes in comm
);

int MPI_Comm_rank(
   MPI_Comm comm, // (in) communicator name (MPI_COMM_WORLD)
   int* rank_p  // (out) rank of calling process in comm
);

MPI COMM WORLD - default communicator

------ 3.1.6 Communication ----
int MPI_Send(
   void* message, // (in) message buffer
   int count, // (in) number of elements in message
   MPI_Datatype datatype, // (in) type of elements in message
   int dest, // (in) rank of destination
   int tag, // (in) message tag
   MPI_Comm comm // (in) communicator
);

int MPI_Recv(
   void* message, // (out) message buffer
   int count, // (in) max number of elements in message
   MPI_Datatype datatype, // (in) type of elements in message
   int source, // (in) rank of source
   int tag, // (in) message tag
   MPI_Comm comm, // (in) communicator
   MPI_Status* status_p // (out) status object
);

-------- Wildcards ---------
MPI_ANY_SOURCE
MPI_ANY_TAG

MPI uses push model


------ 3.1.10 ------
The status_p argument
These can be unknown:
1. the amount of data in the message,
2. the sender of the message, or
3. the tag of the message.

MPI_Status status;
this struct contains the following fields:
MPI_SOURCE
MPI_TAG
MPI_ERROR

We can find the amount of data in the message by using the following function:
int MPI_Get_count(
   MPI_Status* status_p, // (in) status object
   MPI_Datatype datatype, // (in) type of elements in message
   int* count_p // (out) number of elements in message
);

----- Semantics of MPI ----
Buffer and Block 
Buffer: stores the message and return
Block: the process is blocked until the message is sent or received
The exact behavior of MPI Send is determined by the MPI implementation. However, typical implementations have a default “cutoff” message size. If the size of a
message is less than the cutoff, it will be buffered. If the size of the message is greater
than the cutoff, MPI Send will block.
MPI Recv will always block until the message is received.

Follows Nonovertaking rule
first message sent is the first message received from the same sender

----- Pitfalls -----
> MPI_Recv may block forever if the message is never sent - hang



*/