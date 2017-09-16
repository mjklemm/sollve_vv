// RUN: %libomptarget-compile-run-and-check-aarch64-unknown-linux-gnu
// RUN: %libomptarget-compile-run-and-check-powerpc64-ibm-linux-gnu
// RUN: %libomptarget-compile-run-and-check-powerpc64le-ibm-linux-gnu
// RUN: %libomptarget-compile-run-and-check-x86_64-pc-linux-gnu

//===--test_target_map_array_default.c - test default behavior of array map--===//
// 
// OpenMP API Version 4.5 Nov 2015
//
// Whenever a map-type-modifier is not specified in the map clause, the symbol
// is mapped as a tofrom. This test make sure this is satisfied
//
////===----------------------------------------------------------------------===//

#include <omp.h>
#include <stdio.h>

#define N 1000

int main() {
  int compute_array[N];
  int sum = 0, result = 0;
  int i, isHost = -1, exeW = -1;
  
 
  for (i=0; i<N; i++) 
    compute_array[i] = 0;

#pragma omp target map(compute_array) map(tofrom: isHost)
  {
    /*Record where the computation was executed*/
    isHost = omp_is_initial_device();
    exeW = (isHost < 0)? 1 : 0;/*1=device, 0=host*/

    for (i = 0; i < N; i++)
      compute_array[i] = i;
   
  } // End target

  for (i = 0; i < N; i++)
    sum = sum + compute_array[i];    
  
  for (i = 0; i < N; i++)
    result += i;

  if (result == sum){
    if (exeW)
      printf("Test passed on device\n"); 
    else 
      printf("Test passed on host\n"); 
    return 0;
  }
  else {
    if (exeW)
      printf("Test failed on device\n"); 
    else 
      printf("Test failed on host\n"); 

    return 1;
  }

  return 0;
}
