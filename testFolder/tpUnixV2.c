#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


char *articleList[] = {"body","brassiere","pyjama"};
char *vendeurList[] = {"Pierre","Paul","Jacques"}; // 0-1, 2-3,4-5
char *clientList[] = {"Chloe","Elise","Lea"}; // 6-7, 8-9, 10-11
char *caissiereList[] = {"Lilou","Laura","Nadia"}; // 12-13, 14-15, 15-17

int main(){

  char *ch=NULL;
  char *ch1=NULL;
  int readpipe[2];
  int writepipe[2];
  int pipe1, pipe2;
  pipe1=pipe(readpipe);
  pipe2=pipe(writepipe);

  //////////


  int parent_pipe[2];
  int child_pipe[2];
  char buff[50];

  char buffer[100];
  memset(buffer, '\0', 100);

  pid_t pid;


  pid = fork();
  // check pid
  

  // if(pid==0)
  // { //CHILD PROCESS
  //   close(readpipe[1]);
  //   close(writepipe[1]);
  //   read(readpipe[0],ch,sizeof(ch));
  //   printf("\nREAD = %s",ch);
  //   close(readpipe[0]);
  //   ch1="YES";
  //   write(writepipe[1],ch1,sizeof(ch1));
  //   close(writepipe[1]);
  // }
  // else
  // { //PARENT PROCESS
  //   close(writepipe[0]);
  //   close(writepipe[1]);
  //   ch="HI!! YOU THERE";
  //   write(readpipe[1],ch,sizeof(ch));
  //   close(readpipe[1]);
  //   read(writepipe[1],ch1,sizeof(ch1));
  //   printf("\nACK RECEIVED %s",ch1);
  //   close(writepipe[1]);
  // }
  // return 0;

  // if(pid == -1){
  //   // Erreur Fork
  //   perror ("fork( )"); exit (-1);
  // }
  // if(pid == 0){
  //   printf("Dans le fils");
  //   // Code du Fils
  //   close(readpipe[1]);
  //   close(writepipe[1]);
  //   read(readpipe[0],ch,sizeof(ch));
  //   printf("\nVendeur: %s",ch);
  //   close(readpipe[0]);
  //   ///
  //   ch1="Bonjour M. Vendeur";
  //   write(writepipe[1],ch1,sizeof(ch1));
  //   close(writepipe[1]);
  //   /////
  //   read(readpipe[0],ch,sizeof(ch));
  //   printf("\nVendeur: %s",ch);
  //   close(readpipe[0]);
  //   ///
  //   ch1="J'aimerais un BODY";
  //   write(writepipe[1],ch1,sizeof(ch1));
  //   close(writepipe[1]);
  //
  // }
  // else{
  //   printf("Dans le père");
  //   // Code du Père
  //   close(writepipe[0]);
  //   close(writepipe[1]);
  //   ch="Bonjour Client !";
  //   write(readpipe[1],ch,sizeof(ch));
  //   close(readpipe[1]);
  //   ////
  //   read(writepipe[1],ch1,sizeof(ch1));
  //   printf("\nClient: %s",ch1);
  //   close(writepipe[1]);
  //   /////
  //   ch="Qu'est ce qui vous ferez plaisir ?";
  //   write(readpipe[1],ch,sizeof(ch));
  //   close(readpipe[1]);
  //   ///
  //   read(writepipe[1],ch1,sizeof(ch1));
  //   printf("\nClient: %s",ch1);
  //   close(writepipe[1]);
  // }
}
