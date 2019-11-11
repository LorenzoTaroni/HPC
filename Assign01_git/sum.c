#include <stdio.h>

int main(int argc , char *argv[ ]) {

  long long int S = 0, N = argv[1];

  

  for (long long int i = 1; i < N + (long long int)(1); i++) {

    S += i;

  }



  return 0;
}
