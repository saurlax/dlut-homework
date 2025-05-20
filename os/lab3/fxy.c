#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to calculate factorial f(x)
int factorial(int x)
{
  if (x <= 1)
    return 1;
  return x * factorial(x - 1);
}

// Function to calculate Fibonacci f(y)
int fibonacci(int y)
{
  if (y <= 2)
    return 1;
  return fibonacci(y - 1) + fibonacci(y - 2);
}

int main(int argc, char *argv[])
{
  // Check if arguments are provided
  if (argc != 3)
  {
    printf("Usage: %s <x> <y>\n", argv[0]);
    return 1;
  }

  // Convert command line arguments to integers
  int x = atoi(argv[1]);
  int y = atoi(argv[2]);

  // Create pipes for communication
  int pipe_x[2]; // For communicating f(x) result
  int pipe_y[2]; // For communicating f(y) result

  // Create the pipes
  if (pipe(pipe_x) < 0 || pipe(pipe_y) < 0)
  {
    perror("Failed to create pipes");
    exit(EXIT_FAILURE);
  }

  // Create first child process to calculate f(x)
  pid_t pid_x = fork();
  if (pid_x < 0)
  {
    perror("Failed to fork first child");
    exit(EXIT_FAILURE);
  }
  else if (pid_x == 0)
  {
    // Child process for f(x)
    close(pipe_x[0]); // Close read end
    close(pipe_y[0]); // Not used
    close(pipe_y[1]); // Not used

    // Calculate f(x)
    int result_x = factorial(x);
    printf("Child 1 (PID %d): f(%d) = %d\n", getpid(), x, result_x);

    // Send result to parent
    write(pipe_x[1], &result_x, sizeof(result_x));
    close(pipe_x[1]);
    exit(EXIT_SUCCESS);
  }

  // Create second child process to calculate f(y)
  pid_t pid_y = fork();
  if (pid_y < 0)
  {
    perror("Failed to fork second child");
    exit(EXIT_FAILURE);
  }
  else if (pid_y == 0)
  {
    // Child process for f(y)
    close(pipe_y[0]); // Close read end
    close(pipe_x[0]); // Not used
    close(pipe_x[1]); // Not used

    // Calculate f(y)
    int result_y = fibonacci(y);
    printf("Child 2 (PID %d): f(%d) = %d\n", getpid(), y, result_y);

    // Send result to parent
    write(pipe_y[1], &result_y, sizeof(result_y));
    close(pipe_y[1]);
    exit(EXIT_SUCCESS);
  }

  // Parent process
  close(pipe_x[1]); // Close write end
  close(pipe_y[1]); // Close write end

  // Read results from children
  int result_x, result_y;
  read(pipe_x[0], &result_x, sizeof(result_x));
  read(pipe_y[0], &result_y, sizeof(result_y));

  // Close pipes completely
  close(pipe_x[0]);
  close(pipe_y[0]);

  // Calculate final result
  int result = result_x + result_y;
  printf("Parent (PID %d): f(%d,%d) = f(%d) + f(%d) = %d + %d = %d\n",
         getpid(), x, y, x, y, result_x, result_y, result);

  // Wait for children to finish
  waitpid(pid_x, NULL, 0);
  waitpid(pid_y, NULL, 0);

  return EXIT_SUCCESS;
}