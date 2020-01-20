
#define _GNU_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include <time.h>
#include <omp.h>




#if defined(_OPENMP)
#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec + \
		  (double)ts.tv_nsec * 1e-9)

#define CPU_TIME_th (clock_gettime( CLOCK_THREAD_CPUTIME_ID, &myts ), (double)myts.tv_sec + \
		     (double)myts.tv_nsec * 1e-9)
#else

#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec + \
		   (double)ts.tv_nsec * 1e-9)

#endif




void do_the_mandelbrot( const int nx, const int ny, const double xl, const double yl, const double xr, const double yr, const unsigned short imax, unsigned short* M );

typedef enum { DATA_TAG, END_TAG, DONE_TAG} Tags;


int main (int argc, char **argv)
{

  struct  timespec ts;


  double tstart = CPU_TIME;


  // Parse the command line arguments
  if (argc != 8) {
    printf("Usage:   %s <n_x> <n_y> <x_L> <y_L> <x_R> <y_R> <I_max> \n", argv[0]);
    printf("Example: %s 300 300 -2.05 -1.5 0.95 1.5 300 \n", argv[0]);
    exit(EXIT_FAILURE);
  }



    /* MPI INIT */
 
    int rank, size, hs;
    if(MPI_Init(&argc,&argv) != MPI_SUCCESS)
    {
        perror("Unable to initialize MPI\n");
        exit(1);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;




  /* WRITING HEADER ON FILE */

  if (rank == 0) 
  {
    FILE *image_file;   

    image_file = fopen("mandy_mpi.pgm", "w"); 

    fprintf(image_file, "P5\n%s %s\n%s\n", argv[1], argv[2], argv[7]);

    hs = ftell(image_file);                  // Header size in byte

    fclose(image_file);  
  
  }

  
  MPI_Bcast(&hs, 1, MPI_INT, 0, MPI_COMM_WORLD);          // Broadcasting header size hs





    /* Global variables */
    int job[2];
    int done;

    MPI_File image_file;
    int rc;    // Trigger for errors in MPI_File_open





  // Cell subdivision factor
  const double div = 10;

  // The window in the plane
  const double xl = atof(argv[3]);
  const double yl = atof(argv[4]);
  const double xr = atof(argv[5]);
  const double yr = atof(argv[6]);

  // Maximum number of iterations, at most 65535
  const unsigned short imax = atoi(argv[7]);

  // Total Image pixel size 
  const int nx = atoi(argv[1]);
  const int ny = atoi(argv[2]);


    if(rank == 0)
    {

      /*-------------------\ 
      |  MASTER SCHEDULER  |
      \-------------------*/



    // Job counters 
    int k, count;
   
    count = 0;
    job[0] = 0;
    job[1] = 1;



    /* Assign first job to every worker */
    for(k = 1; k < size; k++)
    {
          if(count < div*div -size +1)
          {
              ++count;
              ++job[0];
              if (job[0] > div ) { ++job[1]; job[0] = 1;}

              MPI_Send(&job, 2, MPI_INT, k, DATA_TAG, MPI_COMM_WORLD);     // DATA TAG
          }
          else
          {
              ++count;
              ++job[0];
              if (job[0] > div ) { ++job[1]; job[0] = 1;}

              MPI_Send(&job, 2, MPI_INT, k, END_TAG, MPI_COMM_WORLD);      // END TAG  last data to compute
          }

    }


    if (  div*div > size-1  )
    {

    while(count < div*div )
    {


        MPI_Recv(&done , 1 , MPI_INT, MPI_ANY_SOURCE, DONE_TAG, MPI_COMM_WORLD, &status);
 

          if(count < div*div -size +1)
          {
              ++count;
            //  printf("sono %d e count vale %d e ed esco quando vale %d\n", rank, count, (int)(1/(shrink*shrink)));
              ++job[0];
              if (job[0] > div ) { ++job[1]; job[0] = 1;}

              MPI_Send(&job, 2, MPI_INT, status.MPI_SOURCE, DATA_TAG, MPI_COMM_WORLD);     // DATA TAG
          }
          else
          {
              ++count;
            //  printf("sono %d e count vale %d e ed esco quando vale %d\n", rank, count, (int)(1/(shrink*shrink)));
              ++job[0];
              if (job[0] > div ) { ++job[1]; job[0] = 1;}

              MPI_Send(&job, 2, MPI_INT, status.MPI_SOURCE, END_TAG, MPI_COMM_WORLD);      // END TAG  last data to compute
          }


    }

    }
  


    }
    else
    {

        /*---------\ 
        |  WORKER  |
        \---------*/

      MPI_Datatype type;
      //MPI_Status status;
      //int rank;
      //MPI_Comm_rank(MPI_COMM_WORLD,&rank);

      done = 1;

      // Matrix of pixels
      unsigned short *M;
      void *ptr;
      //int color_depth = 1+((imax>>8)>0);

      // Pixel size of cell 
      const int cell_nx = nx/div;
      const int cell_ny = ny/div;

      // Size of a cell
      const double deltax = (xr-xl)/div;
      const double deltay = (yr-yl)/div;




      MPI_Type_vector(cell_ny, cell_ny, nx, MPI_UNSIGNED_SHORT, &type);
      MPI_Type_commit(&type);



        // allocate memory
        if ( (M = (unsigned short*)malloc( cell_nx*cell_ny * sizeof(unsigned short) )) == NULL )
        {
          printf("I'm sorry, there is not enough memory to host %lu bytes\n", nx*ny * sizeof(unsigned short) );
          return 1;
        }





      while(1)
      {


        MPI_Recv(&job , 2 , MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

       // printf("sono %d e ho ricevuto il job pari a job[0]=%d e job[1]=%d\n", rank, job[0], job[1]);

        // Corners of cell
        double cell_xl = xl + (job[0]-1)*deltax;
        double cell_yl = yr - job[1]*deltay;
        double cell_xr = xl + job[0]*deltax;
        double cell_yr = yr - (job[1]-1)*deltay;


  



          do_the_mandelbrot( cell_nx, cell_ny, cell_xl, cell_yl, cell_xr, cell_yr, imax, M );


          ptr = (void*)M;


          // Writing on file part         


        rc = MPI_File_open(MPI_COMM_SELF, "mandy_mpi.pgm", MPI_MODE_WRONLY, MPI_INFO_NULL, &image_file);

        if (rc) {
            printf( "Unable to open file\n" );fflush(stdout);
        }
 

          MPI_Offset displacement = hs + (((job[0]-1) + (job[1]-1)*ny)*cell_nx)*sizeof(unsigned short); 

       //  printf("sono %d e sto per fare write\n", rank);


          MPI_File_set_view( image_file, displacement, MPI_UNSIGNED_SHORT, type, "native", MPI_INFO_NULL );


       //   MPI_File_write_at( image_file, displacement, M, cell_nx*cell_ny, type, &status);


          MPI_File_write( image_file, M, cell_nx*cell_ny, MPI_UNSIGNED_SHORT, &status);


      MPI_File_close(&image_file);
   

      //  printf("sono %d e ho aperto e chiuso il file\n", rank);
        
        if (status.MPI_TAG == END_TAG) {break;}       // EXITING CONDITION

        
        MPI_Send(&done, 1, MPI_INT, 0, DONE_TAG, MPI_COMM_WORLD);

      //  printf("sono %d e ho inviato done al master con valore %d\n", rank, done);

      }



      MPI_Type_free (&type);
      free(M);

    }    
    
    /* Program Finished */

   //printf("sono %d e sto per arrivare a barrier\n", rank );


    MPI_Barrier(MPI_COMM_WORLD);

    double tend = CPU_TIME;

    printf("%d,%g\n", rank, tend - tstart );

    MPI_Finalize();

    return 0;
}






void do_the_mandelbrot( const int nx, const int ny, const double xl, const double yl, const double xr, const double yr, const unsigned short imax, unsigned short* M ){



  // Precompute pixel width and height
  double dx=(xr-xl)/nx;
  double dy=(yr-yl)/ny;


  #ifdef OMP
  #pragma omp parallel shared(M)
  #endif
  {


  double x, y; // Coordinates of the current point in the complex plane
  double u, v; // Coordinates of the iterated point
  int i,j; // Pixel counters
  int n; // Iteration counter

  #ifdef OMP
  #pragma omp for schedule(dynamic) collapse(2)
  #endif

  for (j = 0; j < ny; ++j) {
    //y = yr - j * dy;
    for(i = 0; i < nx; ++i) {
      double u = 0.0;
      double v= 0.0;
      double u2 = u * u;
      double v2 = v*v;
      x = xl + i * dx;
      // iterate the point
      for (n = 1; n < imax && (u2 + v2 < 4.0); ++n) {
            v = 2 * u * v + yr - j * dy;
            u = u2 - v2 + x;
            u2 = u * u;
            v2 = v * v;
      };
      
      if (n >= imax) {
        // Mandelbrot points

        M[i + nx*j] = (unsigned short)0;

      }
      else {
        // external points
     
        M[i + nx*j] = (unsigned short)n;

      };
    }
  }

  }


}
