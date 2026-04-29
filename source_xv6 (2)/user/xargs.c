#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  char buf[512];
  char *new_argv[MAXARG];
  int i;

  if (argc < 2) {
    fprintf(2, "Usage: xargs <command> [args...]\n");
    exit(1);
  }

  for (i = 1; i < argc; i++) {
    new_argv[i - 1] = argv[i];
  }

  while (1) {
    int k = 0;
    while (1) {
      int n = read(0, &buf[k], 1); 
      if (n <= 0 || buf[k] == '\n') break; 
      k++;
    }

    if (k == 0) break; 

    buf[k] = 0; 
    new_argv[argc - 1] = buf; 
    new_argv[argc] = 0; 

    if (fork() == 0) {
      exec(new_argv[0], new_argv); 
      exit(0);
    } else {
      wait(0); 
    }
  }
  exit(0);
}
