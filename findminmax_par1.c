// ICCS222 :: OS
// Project 0

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <unistd.h>

#define DEBUG_ME printf("LINE number: %d\n", __LINE__ );

int* slide_array(int* arr, int start, int end, int n);

typedef struct {
  int min;
  int max;
} result_t;

// array of numbers
int *arr;

// timing
struct timeval tval_before, tval_after, tval_result;

result_t find_min_and_max(int *subarray, int n) {
  result_t ret;
  ret.min = INT_MAX;
  ret.max = INT_MIN;
  for (int i=0; i<n; i++) {
    if (subarray[i] < ret.min) ret.min = subarray[i];
    if (subarray[i] > ret.max) ret.max = subarray[i];
  }
  return ret;
}

int main(int argc, char** argv) {

  if (argc < 4) {
    printf("Usage: %s <seed> <array_size> <nprocs>\n",argv[0]);
    return 1;
  }

  int nprocs = atoi(argv[3]);
  pid_t* pid_array = (pid_t *) malloc(nprocs * sizeof(pid_t));

  int seed = atoi(argv[1]);
  int n = atoi(argv[2]);
  arr = (int *)malloc(n * sizeof(int));
  // randomly generate n numbers
  srand(seed);
  for (int i=0; i<n; i++) {
    arr[i] = rand();
  }

  result_t partial_result;

  // Start work
  gettimeofday(&tval_before, NULL);

  // need to split array into nprocs pieces
  for (int i = 0; i < nprocs; i++) {
    pid_array[i] = fork();
    if (pid_array[i] == 0) { // child process
      int sub_n = n/nprocs;
      int* sub_arr = (int *) malloc(sub_n * sizeof(int));
      for (int j = 0; j < sub_n; j++) {
        sub_arr[j] = arr[i*sub_n+j];
      }
      partial_result = find_min_and_max(sub_arr, sub_n);
      printf("min = %d, max = %d\n", partial_result.min, partial_result.max);
    }
  }

  result_t r = find_min_and_max(arr, n);

  gettimeofday(&tval_after, NULL);

  // Print out the result and time measurement
  // printf("min = %d, max = %d\n", r.min, r.max);
  timersub(&tval_after, &tval_before, &tval_result);
  printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
}

int* slide_array(int* arr, int start, int end, int n) {
  int* new_arr = (int *) malloc(n * sizeof(int));
  for (int i = start; i <= end; i++) {
    new_arr[i] = arr[i];
  }
  return new_arr;
}
