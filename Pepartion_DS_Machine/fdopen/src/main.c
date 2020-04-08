#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LENGHT 200

void codeFils(int tube[], char dest[]){
    FILE * destFlot;
    FILE * flot;
    char chaine[LENGHT];

    printf("dest %s\n", dest);

    destFlot=fopen(dest, "w");
    if (destFlot==NULL){
        perror("Fopen error\n");
        exit(errno);
    }
/*
    flot=fdopen(tube[0],"r");
    if (flot==NULL){
        perror("fopen error");
        exit(errno);
    }
*/
/*
    while(fgets(chaine,LENGHT,flot)!=NULL){
         fprintf(destFlot,chaine,LENGHT);
    }
*/
   while(read(tube[0],chaine, LENGHT)){
         fprintf(destFlot,chaine,LENGHT);
    }
    //fclose(flot);
    fclose(destFlot);
    close(tube[0]);
    exit(0);
}
void codePere(int tube[], char source[]){
    FILE * flot;
    char chaine[LENGHT];    
    
    printf("source %s\n", source);

    flot=fopen(source,"r");
    if (flot==NULL){
        perror("fopen errori 1 ");
        exit(errno);
    }

    FILE * fic;
    fic= fdopen(tube[1], "w");

    while(fgets(chaine, LENGHT, flot)!=NULL){
        
        fputs(chaine,fic);
        fflush(fic);
  //      fprintf(fic, "%s", chaine);
        //        write(tube[1],chaine, strlen(chaine));
    }

    fclose(flot);
    close(tube[1]);
    wait(NULL);
    exit(0);

}



int main (int argc, char **argv){
    int tube[2];
    char fichier[2][200];

    if (argc != 3){
        fprintf(stderr, "invalid arg\n");
        exit(1);
    }

    if (sscanf(argv[1], "%s",fichier[0])==0){
        fprintf(stderr,"must be a string");
        exit(0); 
    }

    if (sscanf(argv[2], "%s",fichier[1])==0){
        fprintf(stderr,"must be a string");
        exit(0); 
    }


    if (pipe(tube)==-1){
        perror("Error pipe\n");
        exit(errno);
    }

    switch (fork()){
        case -1:
            perror("Fork error\n");
            exit(errno);
            break;

        case 0:
            close(tube[1]);
            codeFils(tube,fichier[1]);
            exit(0); 

            break;
        default:

            close(tube[0]);
            codePere(tube,fichier[0]);
            exit(0); 

    }

   return 0;
}
