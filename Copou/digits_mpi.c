# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include "mpi.h"

int main ( int argc, char *argv[] );
int digits_count ( int n, int id, int p );
void timestamp ( void );

int main ( int argc, char *argv[] )
{
  int i;
  int id;
  int ierr;
  int master = 0;
  int n;
  int n_factor;
  int n_hi;
  int n_lo;
  int p;
  int digits;
  int digit_parts;
  double wtime;
  n_lo = 32768;
  n_hi = 1048576;
  n_factor = 2;
  ierr = MPI_Init ( &argc, &argv );
  ierr = MPI_Comm_size ( MPI_COMM_WORLD, &p );
  ierr = MPI_Comm_rank ( MPI_COMM_WORLD, &id );
  if ( id == master )
  {
    timestamp ( );
    printf ( "\n" );
    printf ( "'DIGITS_MPI\n" );
    printf ( "  C/MPI version\n" );
    printf ( "\n" );
    printf ( "  An MPI example program to count the number of numbers having same first and last digit.\n");
    printf ( "  The number of processes is %d\n", p );
    printf ( "\n" );
    printf ( "         N        digits          Time\n" );
    printf ( "\n" );
  }
  n = n_lo;
  while ( n <= n_hi )
  {
    if ( id == master )
    {
      wtime = MPI_Wtime ( );
    }
    ierr = MPI_Bcast ( &n, 1, MPI_INT, master, MPI_COMM_WORLD );
    digit_parts = digits_count ( n, id, p );
    ierr = MPI_Reduce ( &digit_parts, &digits, 1, MPI_INT, MPI_SUM, master, MPI_COMM_WORLD );
    if ( id == master )
    {
      wtime = MPI_Wtime ( ) - wtime;
      printf ( "  %8d  %8d  %14f\n", n, digits, wtime );
    }
    n = n * n_factor;
  }
  ierr = MPI_Finalize ( );
  if ( id == master ) 
  {
    printf ( "\n");         
    printf ( "DIGIT_MPI - Master process:\n");         
    printf ( "  Normal end of execution.\n"); 
    printf ( "\n" );
    timestamp ( );        
  }
  return 0;
}

int digits_count ( int n, int id, int p )
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

void timestamp ( void )
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  size_t len;
  time_t now;
  now = time ( NULL );
  tm = localtime ( &now );
  len = strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );
  printf ( "%s\n", time_buffer );
  return;
# undef TIME_SIZE
}
