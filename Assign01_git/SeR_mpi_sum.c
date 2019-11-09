#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#define USE MPI

#define MAXCHAR 1000

int main ( int argc , char *argv[ ] )
{

  // times 
  double start_time0, end_time0;
  double start_time_T_read, end_time_T_read; 
  double start_time_T_comp, end_time_T_comp;
  double start_time_T_comm, end_time_T_comm;    
  int myid , numprocs , proc ;
  MPI_Status status;
  MPI_Request request;
  // master process
  int master = 0;
  int tag = 123;
  long long int partial_N, N;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);


 if ( argc <=1) {
    fprintf (stderr , " Usage : mpirun -np n %s (file with N of operations) \n", argv[0] ) ;
    MPI_Finalize() ;
    exit(-1) ;
  }



  if (myid ==0) { 

  // take time of processors after initial I/O operation
    start_time0 = MPI_Wtime();


//##########################################################
                                                          //
    start_time_T_read = MPI_Wtime();                      //
                                                          //
      FILE *fp;                                           //
      char str[MAXCHAR];
      char* filename = argv[1];
   
      fp = fopen(filename, "r");

      if (fp == NULL){
        printf("Could not open file %s",filename);        // reading part
        return 1;
      }

      fgets(str, MAXCHAR, fp); 
    
      N = atoll(str);

      fclose(fp);
                                                          //
                                                          //
     end_time_T_read = MPI_Wtime();                       //
                                                          //
//##########################################################

  }

  if (myid ==0) { //if I am the master send how many number each proc has to sum, i.e. partial_N
    
    //partial_N = (N - N%numprocs)/numprocs;


    partial_N = (long long int)(N/numprocs);


    start_time_T_comm = MPI_Wtime();

    for (proc = 1; proc < numprocs; proc++) {

      MPI_Ssend(&partial_N, 1, MPI_LONG_LONG, proc, tag, MPI_COMM_WORLD);

    }

  }

  else {   // for all the slave receive partial_N /

    MPI_Recv(&partial_N, 1, MPI_LONG_LONG, master, tag,MPI_COMM_WORLD, &status);

  }


  if (myid ==0) { 

    end_time_T_comm = MPI_Wtime();

  }


  //############################################################################################################
                                                                                                              //
  long long int partial_sum = 0;                                                                              //
  long long int i;                                                                                            //
  for (i = 1*(myid + 1); i < partial_N*(myid + 1); i++) {                    //parallel part                             
     
    partial_sum += i; 
                                                                                                              //
  }                                                                                                           //
                                                                                                              //
  //############################################################################################################


  if (myid ==0) { //if I am the master compute remaining operations                     

    long long int j;
    for (j = N - N%numprocs + 1; j < N; j++) {

      partial_sum += j;

    }                                                                                                      
                                                                                                             
  }                                                                                                          




  if (myid ==0) { //if I am the master process gather results from others

    long long int S = partial_sum;


    for (proc = 1; proc < numprocs; proc++) {

      MPI_Recv(&partial_sum, 1, MPI_LONG_LONG, proc, tag, MPI_COMM_WORLD, &status);

      S += partial_sum;

    }


    end_time0=MPI_Wtime();

    //printf ( "\n # of trials = %llu , estimate of pi is %1.9f \n", N*numprocs, pi );
    //printf ( "\n # walltime on master processor : %10.8f \n", end_time - start_time );
    printf ("The sum of numbers between 1 and %llu is %llu \n", N, S);
    printf ( "%10.8f \n", end_time0 - start_time0 );

     FILE * fp1;
     fp1 = fopen ("T_read.txt", "w+");
     fprintf(fp1, "%10.8f", end_time_T_read - start_time_T_read );
     fclose(fp1);
    
    double c;

    start_time_T_comp = MPI_Wtime();
 
    c = 2. + 2.;

    end_time_T_comp = MPI_Wtime();

     FILE * fp2;
     fp2 = fopen ("T_comp.txt", "w+");
     fprintf(fp2, "%10.8f", end_time_T_comp - start_time_T_comp );
     fclose(fp2);

     FILE * fp3;
     fp3 = fopen ("T_comm.txt", "w+");
     fprintf(fp3, "%10.8f", end_time_T_comm - start_time_T_comm );
     fclose(fp3);


  }

  else {   // for all the slave processes send results to the master /

//    printf ( " Processor %d sending results = %llu to master process \n", myid, local_M);
//    int time_to_sleep=1*myid;
//    sleep(time_to_sleep);

    MPI_Ssend(&partial_sum, 1, MPI_LONG_LONG, master, tag, MPI_COMM_WORLD);

    end_time0=MPI_Wtime();

    //printf ( "\n # walltime on processor %i : %10.8f \n",myid, end_time - start_time );
    printf ( "%10.8f \n", end_time0 - start_time0 );
  }

  MPI_Finalize(); // let MPI finish up /

 return 0;
}
