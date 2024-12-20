/* Original test file cnt.c comes from the MRTC WCET benchmark.
  
   Description: 
   Counts non-negative numbers in a matrix.	
   
   Comments:
   Nested loops, well-structured code.
*/
/* Changes:
 * AJ 2014/04/15: Merged patmos/bench changes (PRINT_RESULTS, return check)
 */

#ifdef PRINT_RESULTS
#include <stdio.h>
#endif

#define MAXSIZE 20
#define WORSTCASE 1

// Typedefs
typedef int matrix [MAXSIZE][MAXSIZE];

// Forwards declarations
int main( void );
int Test( matrix );
int Initialize( matrix );
int InitSeed( void );
void Sum( matrix );
int RandomInteger( void );

// Globals
int Seed;
matrix Array;
int Postotal, Negtotal, Poscnt, Negcnt;

// The main function
int main ( void )
{
  InitSeed();
#ifdef PRINT_RESULTS
  printf("cnt: \ncnt:   *** MATRIX SUM AND COUNT BENCHMARK TEST ***\ncnt:\n");
  printf("cnt:RESULTS OF THE TEST:\n");
#endif
  int r = Test(Array);
  if (r!=-1537870) return 1;
  return 0;
}


int Test( matrix Array )
{
  Initialize( Array );

  Sum( Array );

#ifdef PRINT_RESULTS
  printf("cnt:    - Size of array is %d\n", MAXSIZE);
  printf("cnt:    - Num pos was %d and Sum was %d\n", Poscnt, Postotal);
  printf("cnt:    - Num neg was %d and Sum was %d\n", Negcnt, Negtotal);
#endif
  return Negtotal-Postotal;
}


// Intializes the given array with random integers.
int Initialize( matrix Array )
{
  register int OuterIndex, InnerIndex;

  _Pragma("loopbound min 20 max 20")
  for ( OuterIndex = 0; OuterIndex < MAXSIZE; OuterIndex++ ) //100 + 1
    _Pragma("loopbound min 20 max 20")
    for ( InnerIndex = 0; InnerIndex < MAXSIZE; InnerIndex++ ) { //100 + 1
      Array[OuterIndex][InnerIndex] = RandomInteger();
    }

  return 0;
}


// Initializes the seed used in the random number generator.
int InitSeed ( void )
{
  Seed = 0;
  return 0;
}

void Sum( matrix Array )
{
  register int Outer, Inner;

  int Ptotal = 0; /* changed these to locals in order to drive worst case */
  int Ntotal = 0;
  int Pcnt = 0;
  int Ncnt = 0;

  _Pragma("loopbound min 20 max 20")
  for (Outer = 0; Outer < MAXSIZE; Outer++) //Maxsize = 100
    _Pragma("loopbound min 20 max 20")
    for (Inner = 0; Inner < MAXSIZE; Inner++)
#ifdef WORSTCASE
      if (Array[Outer][Inner] >= 0) {
#else
      if (Array[Outer][Inner] < 0) {
#endif
	Ptotal += Array[Outer][Inner];
	Pcnt++;
      }
      else {
	Ntotal += Array[Outer][Inner];
	Ncnt++;
      }

  Postotal = Ptotal;
  Poscnt = Pcnt;
  Negtotal = Ntotal;
  Negcnt = Ncnt;
}


// Generates random integers between 0 and 8095
int RandomInteger( void )
{
   Seed = ((Seed * 133) + 81) % 8095;
   return Seed;
}


