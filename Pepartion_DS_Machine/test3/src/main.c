#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define LINE_LENGHT 250

// 14 h 18


void codeOfTheSon(int tube[], char dest[]){
    FILE *flot,*pipeFlot;
    char ligne[LINE_LENGHT];

    printf("dest son : %s\n",dest);


    flot =fopen(dest,"w");
    if (flot==NULL){
        perror("FOPEN ERROR\n");
        exit(errno);
    }
/*
    pipeFlot=fdopen(tube[0], "r");
    if (pipeFlot==NULL){
        perror("PIPE FLOT ERROR\n");
        exit(errno);
    }
  */  printf("ok\n"); 
    read(tube[0],ligne,strlen(ligne));
    printf("test :%s",ligne); 
/*
    while(fgets(ligne,strlen(ligne),pipeFlot)!=NULL){
        printf("ok2\n");
        fprintf(flot,ligne,strlen(ligne));
        printf("receive %s\n",ligne);
    }
*/

    close(tube[0]);
   // fclose(pipeFlot);
    fclose(flot);
    exit(0);
}

void codeOfTheFather(int tube[], char source[]){
    printf("source father %s\n", source);

    FILE *flot;
    char ligne[LINE_LENGHT];

    flot =fopen(source,"r");
    if (flot==NULL){
        perror("FOPEN ERROR\n");
        exit(errno);
    }

    fgets(ligne,LINE_LENGHT,flot);       
    while(!feof(flot)){
        fgets(ligne,LINE_LENGHT,flot);       
        write(tube[1], &ligne, strlen(ligne));
    }



    close(tube[1]);

    fclose(flot);
    exit(0);

}



int main(int argc , char **argv){
    int tube[2];
    char file[2][150]; // source is the first dest is the second
    int z;



    if (argc !=3){
        fprintf(stderr,"ARG ERROR\n");
        exit(1);
    }


    for(z=1; z < argc ; z++ ){
        if(sscanf(argv[z],"%s",&file[z-1]) == 0){
            fprintf(stderr, "ARGUMENT MUST BE A STRING\n");
            exit(1);
        }
    }

    if(pipe(tube)==-1){
        perror("PIPE ERROR");
        exit(1);
    }

    switch(fork()){
        case -1:
            perror("PIPE ERROR");
            exit(errno);
            break;

        case 0:
            close(tube[1]);
            codeOfTheSon(tube,file[1]); 
            exit(0);
            break;

        default:
            close(tube[0]);
            codeOfTheFather(tube,file[0]);
            exit(0);
    }
}
