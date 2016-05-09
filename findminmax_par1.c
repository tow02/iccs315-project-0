// ICCS222 :: OS
// Project 0

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>

#define DEBUG_ME printf("LINE number: %d\n", __LINE__ );

int **create_pipe(int nprocs);
void destroy_pipes(int **fd, int nprocs);

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

  int close_status;

  int read_status;
  int write_status;

  int pipe_status;

  int nprocs = atoi(argv[3]);
  pid_t *pid_array = (pid_t *) malloc(nprocs * sizeof(pid_t));

  int seed = atoi(argv[1]);
  int n = atoi(argv[2]);
  arr = (int *)malloc(n * sizeof(int));
  // randomly generate n numbers
  srand(seed);
  for (int i=0; i<n; i++) {
    arr[i] = rand();
  }

  result_t *result_array = (result_t *) malloc(nprocs * sizeof(result_t));

  int current_min = arr[0];
  int current_max = arr[0];

  int *min_max_partial;

  int **fd = create_pipe(nprocs);

  // Start work
  gettimeofday(&tval_before, NULL);

  // need to split array into nprocs pieces
  for (int i = 0; i < nprocs; i++) {
    // int fd[2];
    pipe_status = pipe(fd[i]);
    if (pipe_status == -1) {
      printf("pipe() error\n");
      exit(1);
    }

    else {
      pid_array[i] = fork();

      if (pid_array[i] == -1) {
        perror("fork");
        exit(1);
      }

      else if (pid_array[i] == 0) { // child process

        int sub_n = n/nprocs;
        int *sub_arr = (int *) malloc(sub_n * sizeof(int));
        for (int j = 0; j < sub_n; j++) {
          sub_arr[j] = arr[i*sub_n+j];
        }

        result_array[i] = find_min_and_max(sub_arr, sub_n);

        min_max_partial = (int *) malloc(2 * sizeof(int));
        min_max_partial[0] = result_array[i].min;
        min_max_partial[1] = result_array[i].max;

        /* Child process closes up input side of pipe */
        close_status = close(fd[i][0]);
        if (close_status == -1) {
          // error
          printf("close() error\n");
          exit(1);
        }
        // write_status = write(fd[i][1], min_max_partial, sizeof(min_max_partial));
        write_status = write(fd[i][1], &result_array[i], sizeof(result_array[i]));
        if (write_status == -1) {
          printf("write() error\n");
          exit(1);
        }
        // free(min_max_partial);
        exit(0);
      }

      else {
        /* Parent process closes up output side of pipe */
        close_status = close(fd[i][1]);
        if (close_status == -1) {
          printf("close() error\n");
          exit(1);
        }

        // read_status = read(fd[i][0], min_max_partial, sizeof(min_max_partial));
        read_status = read(fd[i][0], &result_array[i], sizeof(result_array[i]));
        if (read_status == -1) {
          printf("read() error\n");
          exit(1);
        }
        if (current_min > result_array[i].min) {
          current_min = result_array[i].min;
        }

        if (current_max < result_array[i].max) {
          current_max = result_array[i].max;
        }
      }
    }
  }

  // result_t r = find_min_and_max(arr, n);

  gettimeofday(&tval_after, NULL);

  // Print out the result and time measurement
  // printf("min = %d, max = %d\n", r.min, r.max);
  printf("min = %d, max = %d\n", current_min, current_max);
  timersub(&tval_after, &tval_before, &tval_result);
  printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

  destroy_pipes(fd, nprocs);
}

int **create_pipe(int nprocs) {
  int **fd = (int **) malloc(nprocs * sizeof(int*));
  for (int i = 0; i < nprocs; i++) {
    fd[i] = (int *) malloc(2 * sizeof(int));
  }
  return fd;
}

void destroy_pipes(int **fd, int nprocs) {
  for (int i=0; i<nprocs; i++) {
    free(fd[i]);
  }
  free(fd);
}
