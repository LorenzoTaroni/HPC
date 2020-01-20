#define _GNU_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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




void write_pgm_image( void *image, int maxval, int xsize, int ysize, const char *image_name);



int main( int argc, char **argv ){

  #ifdef TIME
  struct  timespec ts;
  double tstart = CPU_TIME;
  #endif

  // Parse the command line arguments
  if (argc != 8) {
    printf("Usage:   %s <n_x> <n_y> <x_L> <y_L> <x_R> <y_R> <I_max> \n", argv[0]);
    printf("Example: %s 300 300 -2.05 -1.5 0.95 1.5 300 \n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // The window in the plane
  const double xl = atof(argv[3]);
  const double yl = atof(argv[4]);
  const double xr = atof(argv[5]);
  const double yr = atof(argv[6]);

  // Maximum number of iterations, at most 65535
  const unsigned short imax = atoi(argv[7]);

  // Image size 
  const int nx = atoi(argv[1]);
  const int ny = atoi(argv[2]);



  // Precompute pixel width and height
  double dx=(xr-xl)/nx;
  double dy=(yr-yl)/ny;

  // Matrix of pixels
  unsigned short *M;
  

  // allocate memory
  if ( (M = (unsigned short*)malloc( nx*ny * sizeof(unsigned short) )) == NULL )
    {
      printf("I'm sorry, there is not enough memory to host %lu bytes\n", nx*ny * sizeof(unsigned short) );
      return 1;
    }


  #ifdef OMP
  #pragma omp parallel shared(M)
  #endif
  {

  #ifdef Tth
  struct  timespec myts;
  double mystart = CPU_TIME_th;
  #endif

  double x, y; // Coordinates of the current point in the complex plane
  double u, v; // Coordinates of the iterated point
  int i,j; // Pixel counters
  int n; // Iteration counter

  #ifdef OMP
  #pragma omp for collapse(2)
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

  #ifdef Tth
  double myend = CPU_TIME_th;
  printf("<%g>\n", myend - mystart);
  #endif

  }


  

  void *ptr;
 
  ptr = (void*)M;

  write_pgm_image( ptr, imax, nx, ny, "mandy.pgm" );


  free(M);

  #ifdef TIME
  double tend = CPU_TIME;
  printf("<%g>\n", tend - tstart);
  #endif

  return 0;
}





void write_pgm_image( void *image, int maxval, int xsize, int ysize, const char *image_name)
{
  FILE* image_file; 
  image_file = fopen(image_name, "w"); 
  
  // Writing header
  // The header's format is as follows, all in ASCII.
  // "whitespace" is either a blank or a TAB or a CF or a LF
  // - The Magic Number (see below the magic numbers)
  // - the image's width
  // - the height
  // - a white space
  // - the image's height
  // - a whitespace
  // - the maximum color value, which must be between 0 and 65535
  //
  // if he maximum color value is in the range [0-255], then
  // a pixel will be expressed by a single byte; if the maximum is
  // larger than 255, then 2 bytes will be needed for each pixel
  //

  int color_depth = 1+((maxval>>8)>0);       // 1 if maxval < 256, 2 otherwise

  fprintf(image_file, "P5\n%d %d\n%d\n", xsize, ysize, maxval);
  
  // Writing file
  fwrite( image, color_depth, xsize*ysize, image_file);  

  fclose(image_file); 
  return ;
}
