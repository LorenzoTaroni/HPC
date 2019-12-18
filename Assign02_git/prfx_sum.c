#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <math.h>

#define _GNU_SOURCE

#define N_default 10

#if defined(_OPENMP)
#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec + \
		  (double)ts.tv_nsec * 1e-9)

#define CPU_TIME_th (clock_gettime( CLOCK_THREAD_CPUTIME_ID, &myts ), (double)myts.tv_sec + \
		     (double)myts.tv_nsec * 1e-9)
#else

#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec + \
		   (double)ts.tv_nsec * 1e-9)

#endif

double* prfxsum_S( double* x, int N);

double* prfxsum_P( double* x, int N);

//double* prfxsum_P_better( double* x, int N);


int main( int argc, char **argv )
{

  int     N        = N_default;
  //int     nthreads = 1;
  
  struct  timespec ts;
  double *x, *a;



  /*  -----------------------------------------------------------------------------
   *   initialize 
   *  -----------------------------------------------------------------------------
   */

  // check whether some arg has been passed on
  if ( argc > 1 ) {
    N = atoi( *(argv+1) );

   //printf ("%d", N);
  }

  double wtstart = CPU_TIME;

  // allocate memory
  if ( (x = (double*)malloc( N * sizeof(double) )) == NULL )
    {
      printf("I'm sorry, there is not enough memory to host %lu bytes\n", N * sizeof(double) );
      return 1;
    }

#ifdef OMP
#pragma omp parallel for
#endif
  // initialize the array
  for ( int ii = 0; ii < N; ii++ )
    x[ii] = (double)(1*ii+1);                                
                            


  /*  -----------------------------------------------------------------------------
   *   calculate
   *  -----------------------------------------------------------------------------
   */
  
  double tstart = CPU_TIME;

#ifndef OMP

  a = prfxsum_S( x, N );

#else

  a = prfxsum_P( x, N );

  //a = prfxsum_P_better( x, N );

#endif

  double tend = CPU_TIME;                              

  /*  -----------------------------------------------------------------------------
   *   finalize
   *  -----------------------------------------------------------------------------
   */

  /*printf("Sum is %g, process took %g of wall-clock time\n\n"
       "<%g> sec of avg thread-time\n"
       "<%g> sec of min thread-time\n",
       S, tend - tstart, th_avg_time/nthreads, th_min_time );*/

  for (int i = 0; i < N; ++i) {

    printf("%g ",a[i]);

  }
  

  printf("%g", a[N-1]);
  

  //printf("\nTime: %g\n", tend - tstart);

  //printf("%g, <%g>, <%g>, %g\n", a[N-1], tend - wtstart, th_avg_time/nthreads, th_min_time);

  printf("%g, <%g>\n", a[N-1], tend - wtstart);
  
  free( x );
  free( a );
  return 0;
}



double* prfxsum_S( double* x, int N){

  double *a;

  if ( (a = (double*)malloc( N * sizeof(double) )) == NULL )
  {
    printf("I'm sorry, there is not enough memory to host %lu bytes\n", N * sizeof(double) );
    return (double*)0;
  }

  a[0] = x[0];  

  for (int i = 1; i < N; ++i) {

    a[i] = a[i-1] + x[i];

  }

  return a;
}





/*
double* prfxsum_P_better( double* x, int N){

double g = log10(N)/log10(2);

// Up Sweep fase

  for (int d = 0; (double)(d) < g-1; ++d){

   #pragma omp parallel for   
    for (int k = 0; k < N-1; k += (int)(2**(double)(d+1)) ){

      x[k+(int)(2**(double)(d+1)+1-1)] = x[k+(int)(2**(double)(d)-1)] + x[k+(int)(2**(double)(d))];

    }
  }



// Down Sweep fase

  x[N-1] = 0;

  for (int d = (int)(g)-1; d > 0; --d){

   #pragma omp parallel for  
    for (int k = 0; k < N-1; k += (int)(2**(double)(d) + 1)){

      double t = x[k+(int)(2**(double)(d)-1)];
      x[k+(int)(2**(double)(d)-1)] = x[k+(int)(2**(double)(d))];
      x[k+(int)(2**(double)(d))] = t + x[k+(int)(2**(double)(d))];

    }

  }

  return x;
}
*/





double* prfxsum_P( double* x, int N){

  double *a, *y, *b;

  if ( (a = (double*)malloc( N * sizeof(double) )) == NULL )
  {
    printf("I'm sorry, there is not enough memory to host %lu bytes\n", N * sizeof(double) );
    return (double*)0;
  }

  if (N == 0) {return 0;}

  a[0] = x[0];

  if (N == 1) {return a;}

  a[1] = x[0] + x[1];

  if (N/2 == 1) { a[N-1] = a[N-2] + x[N-1]; return a;}                                               // Stop of recursion

  if ( (y = (double*)malloc( (N/2) * sizeof(double) )) == NULL )
  {
    printf("I'm sorry, there is not enough memory to host %lu bytes\n", N * sizeof(double) );
    return (double*)0;
  }


  #pragma omp parallel for
  for (int i = 0; i < N/2; ++i) {

    y[i] = x[2*i] + x[2*i + 1];

  }

  if ( (b = (double*)malloc( (N/2) * sizeof(double) )) == NULL )
  {
    printf("I'm sorry, there is not enough memory to host %lu bytes\n", N * sizeof(double) );
    return (double*)0;
  }

  b = prfxsum_P( y, N/2 );

  free( y );


  #pragma omp parallel for
  for (int i = 2; i < N; i += 2) {

    a[i] = b[(i+1)/2 - 1] + x[i];
    a[i+1] = b[(i+1)/2];

  }

  a[N-1] = a[N-2] + x[N-1];

  //free( b );

  return a;

}
