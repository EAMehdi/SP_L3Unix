#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

void codeOfTheSon(int tube[], char filename[]){
    int nb_log, i;
    int somme=0,val;

    FILE *flot;
       //fgets(&val, sizeof(val), flot);
    read(tube[0],&nb_log, sizeof(int));
    for (i=0; i<nb_log; i++){
        read(tube[0],&val, sizeof(int));
        printf("Reception : %d\n", val);
        somme+=val;
    }
    printf("Somme: %d\n",somme); 

    flot = fopen(filename,"w");
    if(flot==NULL){
        perror("FDOPEN ERROR");
        exit(errno);
    }

    fprintf(flot,"%c",somme);

    close(tube[0]);
    fclose(flot);
    exit(1);
}


void codeOfTheFather(int tube[], int nb_list[],int nb_log){
    int i;
    write(tube[1],&nb_log,sizeof(int));
    for (i=0; i < nb_log ; i++){
        printf("send %d\n",nb_list[i]);
        write(tube[1],&nb_list[i],sizeof(int));
    }

    close(tube[1]);
    exit(1);
}


int main(int argc, char **argv){
    int tube[2];
    char file_name[50];
    int nb_list[100];
    int i, val,nb_log;

    if (argc <= 2){
        fprintf(stderr,"Arguments Error\n");
        //call usage function 
        exit(1);
    }    

    strcpy(file_name,argv[1]);
    i=2;
    nb_log=0;
    while(i < argc){
        if(sscanf(argv[i],"%d\n",&val)==0){
            fprintf(stderr,"ARGUMENTS INVALID\n");
            exit(1);
        } 
         
        nb_list[i-2]=val;
        i++;
        nb_log++;
    }
/*
    printf("affichage liste\n");

    for (i=0; i <argc-2; i++)
        printf("%d\n",nb_list[i]);
*/
    if (pipe(tube)==-1){
        perror("PIPE ERROR");
        exit(errno);
    }

    switch(fork()){
        // error fork
        case -1:
            exit(errno);
            break;

            //son code
        case 0:
            close(tube[1]);
            codeOfTheSon(tube,file_name);
            exit(1);
            break;

            // father code
        default:
            close(tube[0]);
            codeOfTheFather(tube,nb_list,nb_log);
            exit(1);
    }
    return 0;
}

