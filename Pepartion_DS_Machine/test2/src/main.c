#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void codeOfTheSon(int tube[],char filename[]){ 
//    printf("size of :%d\n",sizeof(filename));
    struct stat buf;

    printf("filename : %s\n",filename);
    if (stat(filename, &buf) == -1){
        perror("STAT ERROR");
        exit(errno);
    }
 
    printf("envoie de :%u\n",buf.st_ino);
    //int test = (int) buf.st_ino;
    write(tube[1], &(buf.st_ino), sizeof(unsigned));
//    write(tube[1],test,sizeof(int));

    close(tube[1]);
    exit(0);
}

void codeOfTheFather(int tube[]){
//    char chaine[150];
    long val;
    read(tube[0],&val, sizeof(long));
    printf("Reception inode : %u\n", val);


    close(tube[0]);
    exit(0);
}

int main(int argc, char **argv){
    int tube[2];
    char filename[150];

    if( argc <2){
        fprintf(stderr,"ARGUMENTS INVALIDi\n");
        exit(1);
    }
    
    if(sscanf(argv[1],"%s", &filename) == 0){
       fprintf(stderr,"INVALID ARGUMENTS MUST BE A STRING"); 
       exit(1);
    }
    

    if(pipe(tube) == -1){
        perror("PIPE ERROR :");
        exit(errno);
    }

    switch(fork()){
        //error
        case -1:
            perror("FORK ERROR :");
            exit(errno);
            break;
        
        //son
        case 0:
            close(tube[0]);
            codeOfTheSon( tube ,filename);
            exit(0);
            break;
        
        default:
            close(tube[1]);
            codeOfTheFather(tube);
            exit(0);

    }

    return 0;

}
