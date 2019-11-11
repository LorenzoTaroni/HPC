#include <stdio.h>

int main(int argc , char *argv[ ]) {

  long long int S = 0, N = 6000000000;

  long long int i;
  
  for (i = 1; i < N + (long long int)(1); i++) {

    S += i;

  }

  printf( "%llu \n ", S);


  return 0;
}
