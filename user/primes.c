#include "kernel/types.h"
#include "user/user.h"


void runprime(int readfd) {
  int mynum = 0;
  int passnum = 0;
  int forked = 0;
  int pipes[2];

  while (1) {
    int read_result = read(readfd, &passnum, 4);

    // means nothing to read
    if (read_result == 0) {
      close(readfd);
      if (forked) {
        close(pipes[1]);
        int child_pid;
        wait(&child_pid);
      }
      exit(0);
    }

    if (mynum == 0) {
      mynum = passnum;
      printf("prime %d\n", mynum);
    }

    if (passnum % mynum != 0) {
      if (!forked) {
        pipe(pipes);
        forked = 1;

        int res = fork();

        if (res == 0) {
          // child
          close(pipes[1]);
          close(readfd);
          runprime(pipes[0]);
        } else {
          // parent
          close(pipes[0]);
        }
      }
      write(pipes[1], &passnum, 4);
    }
  }
}

int main(int argc, char *argv[]) {
  int pipes[2];
  pipe(pipes);

  for (int i = 2; i <= 35; i++) {
    write(pipes[1], &i, 4);
  }
  close(pipes[1]);
  runprime(pipes[0]);
  exit(0);
}