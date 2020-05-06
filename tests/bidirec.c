#include<stdio.h>
#include<unistd.h>

void fils(int p1[2], int p2[2]){

  int i, nombre, nbLu;
  close(p1[1]); //ferme P1 en écriture
  close(p2[0]); // ferme p2 en ecture
  printf("What\n");
  nbLu = read(p1[0],&nombre,sizeof(int));
  printf("Hey\n");
  while(nbLu == sizeof(int)){
    nombre= nombre*2;
    printf("Quoi\n");
    printf("Les doubles sont %d\n",nombre);
    write(p2[1],&nombre,sizeof(int));
    nbLu=read(p1[0],&nombre,sizeof(int));
  }
  close(p1[0]);
  close(p2[1]);
  exit(0);
}

int main(){
  int i, nombre, p1[2],p2[2];
  pipe(p1);
  pipe(p2);

  if(fork() == 0){
    fils(p1,p2);
  }
  else{
    close(p1[0]);
    close(p2[1]);

    printf("Comment\n");
    for(i=0; i < 5; i ++){
      printf("Entrez un entier\n");
      scanf(" %d",&nombre);
      write(p1[1],&nombre,sizeof(int));
    }
    close(p1[1]);

    printf("Bizarre\n");
    printf("Les doubles sont:\n");
    for(i=0; i < 5; i ++){
      read(p2[0],&nombre,sizeof(int));
      printf("BUG ? %d",&nombre);
    }
    printf("\n");
    close(p2[0]);
  }
  return 0;
}
