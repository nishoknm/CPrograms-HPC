# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>

int digits ( int n, int id, int p );

int main ( int argc, char *argv[] )
{
  int i;
  int n;
  int n_factor;
  int n_hi;
  int n_lo;
  int primes;
  int total;

  n_lo = 32768;
  n_hi = 100000000;
  n_factor = 2;

  printf ( "\n" );
  printf ( "'PRIME_MPI\n" );
  printf ( "  C/MPI version\n" );
  printf ( "\n" );
  printf ( "  An MPI example program to count the number of primes.\n" );
  printf ( "\n" );
  printf ( "         N        digits\n" );
  printf ( "\n" );

  n = n_lo;

  while ( n <= n_hi )
  {
    total = digits ( n, 1, 1 );
    printf ( "  %8d  %8d\n", n, total );
    n = n * n_factor;
  }
  return 0;
}

int digits ( int n, int id, int p )
{
  int i;
  int j;
  int lastDigit, firstDigit;
  int total;

  total = 0;

  for ( i = id; i <= n; i = i + p )
  {
    lastDigit = i % 10;
    firstDigit = i;
    while (firstDigit >= 10)
    {
        firstDigit /= 10;
    }
    if(firstDigit == lastDigit) total ++;
  }
  return total;
}