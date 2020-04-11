
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>

int main() {
  //for parent to child
  int parentToChild[2];
  //for child to parent
  int childToParent[2];

  //string data
  char readBuffer[120];
  pipe(parentToChild);
  pipe(childToParent);
  int pid, pidFils;

  pid = fork();
  if(pid == -1){
    printf("Error Fork");
    perror ("fork( )");
    exit (-1);
  }

  if(pid == 0){
    // fils

    printf("Dans le premier fils: [son] pid %d from [parent] pid %d\n",getpid(),getppid());
    printf("%d\n",getpid());

    pidFils = fork();
    if(pidFils == -1){
      printf("Error Fork");
      perror ("fork( )");
      exit (-1);
    }

    if(pidFils == 0){
      // fils
      printf("Petit Fils [son] pid %d from [parent] pid %d\n",getpid(),getppid());
      printf("%d\n",getpid());

      pidFils = fork();


    }

    if(pidFils !=0 ){
      printf("Fils Dans le code du petit Fils [son] pid %d from [parent] pid %d\n",getpid(),getppid());
      printf("%d\n",getpid());
      wait(NULL);
    }

  }

  if(pid !=0 ){
    printf("Pere principale \n",getpid(),getppid());
    printf("%d\n",getpid());
    wait(NULL);
  }
  printf("Hello World\n");
  return 0;
}
