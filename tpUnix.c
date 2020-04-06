/******************************************************************************
 * E/S synchrones non Bloquantes par pipe
 * ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#define MSGSIZE 9

char* msg1 ="bonjour !";
char* msg2 ="au revoir";

void parent_read(int p[]);
void child_write(int p[]);

int main()
{
	int p[2];
	printf("Père. Début.\n");
	printf("Père. Création d'un tube.\n");
	if (pipe(p) < 0) 
	{
		printf("Père. Tubes non supportés. Fin\n");
		exit(1);
	}
	printf("Père. Activation du mode non bloquant.\n");
	/*if (fcntl(p[0], F_SETFL, O_NONBLOCK) < 0)
	 * 	{
	 * 			printf("Père. Mode non bloquant indisponible. Fin\n");
	 * 					exit(2);
	 * 						}*/
	printf("Père. Création Fils.\n");
	switch (fork()) 
	{
		case -1:
			printf("Fork impossible.\n");
			exit(3);
		case 0:
			printf("Fils. Début.\n");
			child_write(p);
			break;
		default:
			parent_read(p);
			break;
	}
	return 0;
}


void parent_read(int p[])
{
	int nread;
	char buf[MSGSIZE];
	printf("Père. Fermeture accès écriture du tube.\n");
	close(p[1]);
	printf("Père. Boucle de lecture.\n");
	while (1) {
		printf(" Père. Lecture tube. ");
		nread = read(p[0], buf, MSGSIZE);
		switch (nread) {
			case -1:
				if (errno == EAGAIN) 
				{
					printf("Tube vide.\n");
					sleep(1);
					break;
				} else {
					perror("read");
					exit(4);
				}
			case 0:
				printf("Fin de communication reçue.\n");
				printf("Père. Fermeture accès lecture du tube.\n");
				close(p[0]);
				printf("Père. Fin.\n");
				exit(0);
			default:
				printf("Message reçu. Taille = %d. Contenu = \"%s\".\n", nread, buf);
		}
	}
}


void child_write(int p[])
{
	printf("Fils. Fermeture accès lecture du tube.\n");
	close(p[0]);

	printf("Fils. Boucle d'écriture.\n");
	for (int i = 0; i < 3; i++) 
	{
		printf(" Fils. écriture tube. ");
		write(p[1], msg1, MSGSIZE);
		printf("Message émis. Taille = %d. Contenu = \"%s\".\n", MSGSIZE, msg1);
		sleep(3);
	}
	printf("Fils. écriture tube. ");
	write(p[1], msg2, MSGSIZE);
	printf("Message émis. Taille = %d. Contenu = \"%s\".\n", MSGSIZE, msg2);
	printf("Fils. Fin émise.\n");
	printf("Fils. Fin.\n");
	exit(0);
}

