// ICCS222 :: OS
// Project 0

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
  int *sub_arr;
  int length;
} package_subarray;

typedef struct {
  int min;
  int max;
} result_t;

// array of numbers
int *arr;

// timing
struct timeval tval_before, tval_after, tval_result;

int current_min;
int current_max;

// result_t find_min_and_max(int *subarray, int n) {
//   result_t ret;
//   ret.min = INT_MAX;
//   ret.max = INT_MIN;
//   for (int i=0; i<n; i++) {
//     if (subarray[i] < ret.min) ret.min = subarray[i];
//     if (subarray[i] > ret.max) ret.max = subarray[i];
//   }
//   return ret;
// }

void *find_min_and_max(void *arg) {
  result_t ret;
  ret.min = INT_MAX;
  ret.max = INT_MIN;

  // arg[0]:sub_arr; arg[1]:length;
  package_subarray *psa = arg;
  int *subarray = psa[0].sub_arr;
  int length = psa[0].length;

  for (int i = 0; i < length; i++) {
    if (subarray[i] < ret.min) {
      ret.min = subarray[i];
    }
    if (subarray[i] > ret.max) {
      ret.max = subarray[i];
    }
  }
  pthread_mutex_lock(&mutex1);
  if (current_min > ret.min) {
    current_min = ret.min;
  }
  if (current_max < ret.max) {
    current_max = ret.max;
  }
  pthread_mutex_unlock(&mutex1);
}

int main(int argc, char** argv) {
  if (argc < 4) {
    printf("Usage: %s <seed> <array_size> <nprocs>\n",argv[0]);
    return 1;
  }

  int nprocs = atoi(argv[3]);

  int seed = atoi(argv[1]);
  int n = atoi(argv[2]);
  arr = (int *)malloc(n * sizeof(int));
  // randomly generate n numbers
  srand(seed);
  for (int i=0; i<n; i++) {
    arr[i] = rand();
  }

  current_min = arr[0];
  current_max = arr[0];

  pthread_t *thread_arr = (pthread_t *) malloc(nprocs * sizeof(pthread_t));

  // Start work
  gettimeofday(&tval_before, NULL);

  // create threads
  for (int i=0; i<nprocs; i++) {
    int sub_n = n/nprocs;
    package_subarray *psa = (package_subarray *) malloc(1 * sizeof(package_subarray));
    if (n%nprocs == 0 && !i) {
      psa[0].sub_arr = arr+(i*sub_n);
      psa[0].length = sub_n;
    }
    else {
      psa[0].sub_arr = arr+(i*sub_n);
      psa[0].length = sub_n+(n%nprocs);
    }

    int thread_status = pthread_create(&thread_arr[i], NULL, &find_min_and_max, (void*)psa);
    if (thread_status != 0) {
      printf("ERROR CREATE THREAD\n");
      exit(1);
    }
  }

  for (int j=0; j<nprocs; j++) {
    pthread_join(thread_arr[j], NULL);
  }

  gettimeofday(&tval_after, NULL);
  // Print out the result and time measurement
  printf("min = %d, max = %d\n", current_min, current_max);
  timersub(&tval_after, &tval_before, &tval_result);
  printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
}
