// ICCS222 :: OS
// Project 0

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

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
    if (argc < 3) {
        printf("Usage: %s <seed> <array_size>\n",argv[0]);
        return 1;
    }
    int seed = atoi(argv[1]);
    int n = atoi(argv[2]);
    arr = (int *)malloc(n * sizeof(int));
    // randomly generate n numbers
    srand(seed);
    for (int i=0; i<n; i++) {
        arr[i] = rand();
    }
    // Start work
    gettimeofday(&tval_before, NULL);
    result_t r = find_min_and_max(arr, n);
    gettimeofday(&tval_after, NULL);
    // Print out the result and time measurement
    printf("min = %d, max = %d\n", r.min, r.max);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
}
