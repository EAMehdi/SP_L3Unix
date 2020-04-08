#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define LENGHT 200

// 10 h 07 -->10 h 57

void codeOfTheFather(int tube[], char filename[]){
    char ligne[LENGHT];
    FILE * flot;
    flot = fopen(filename, "r");
    if( flot==NULL){
        perror("FOPEN ERROR\n");
        exit(errno);
    } 
    
    
    while( fgets(ligne,LENGHT,flot)!=NULL){
        
        write(tube[1],ligne, strlen(ligne));
    }

    close(tube[1]);
    fclose(flot);
    exit(0);
}

void codeOfTheSon(int tube[]){
    char ligne[LENGHT];
    FILE * flot;
    FILE * flotfinal;
    int cpt=0;
    
    flot = fdopen(tube[0],"r");
    if (flot==NULL){
        perror("FDOPEN ERROR\n");
        exit(errno);
    }
    flotfinal=fopen("result.txt", "w");
    if(flotfinal == NULL){
        perror("FOPEN ERROR\n");
        exit(errno);
    }


    printf("\t--------MON CAT ----------\n");
    while(fgets(ligne,LENGHT, flot )!=NULL){
        cpt++;
        printf("%s",ligne);
        fprintf(flotfinal,ligne);
    }
    printf("NB LIGNE: %d\n",cpt);
    close(tube[0]);
    exit(0);
}


int main(int argc, char ** argv){
    int tube[2];
    char filename[LENGHT];

    if ( argc !=2 ) {
        fprintf(stderr, "ARG ERRORR");
        //usageFunction();
        exit(1);
    }

    if(sscanf(argv[1],"%s",filename)==0){
        fprintf(stderr,"INVALID ARGUMENT\n");
        exit(1);
    }

    if (pipe(tube)==-1){
        perror("PIPE ERROR\n");
        exit(errno);
    }

    switch(fork()){
        case -1:
            perror("FORK ERROR\n");
            exit(errno);
            break;
        case 0:
            close(tube[1]);
            codeOfTheSon(tube);
            exit(0);
            break;
        default:
            close(tube[0]);
            codeOfTheFather(tube,filename);
            exit(0);

    }



}
