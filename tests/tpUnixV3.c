#include<stdio.h>
#include<unistd.h>
#include<string.h>
void startMenu();

int main() {
	char *articleList[] = {"body","brassiere","pyjama"};
	char *vendeurList[] = {"Pierre","Paul","Jacques"}; // 0-1, 2-3,4-5
	char *clientList[] = {"Chloe","Elise","Lea"}; // 6-7, 8-9, 10-11
	char *caissiereList[] = {"Lilou","Laura","Nadia"}; // 12-13, 14-15, 15-17

	char article[20];
	char vendeur[20];
	char client[20];
	char caissiere[20];

   int pipefds1[2], pipefds2[2];
   int returnstatus1, returnstatus2;
   int pid;
   char pipe1writemessage[20] = "Bonjour !";
   char pipe2writemessage[20] = "Bien le bonjour !";
   char pipe3writemessage[20] = "Que voulez-vous ?";
   char pipe4writemessage[20] = "Je veux un body :)";
   char readmessage[20];
   int varTest = 99;
   int varTest2;

   startMenu(&article);
   printf("Article dans main : %s\n",article);

   returnstatus1 = pipe(pipefds1);

   if (returnstatus1 == -1) {
      printf("Unable to create pipe 1 \n");
      return 1;
   }
   returnstatus2 = pipe(pipefds2);

   if (returnstatus2 == -1) {
      printf("Unable to create pipe 2 \n");
      return 1;
   }
   pid = fork();

   if (pid != 0)  {  // Parent process
      close(pipefds1[0]); // Close the unwanted pipe1 read side
      close(pipefds2[1]); // Close the unwanted pipe2 write side

      printf("In Parent: Writing to pipe 1 – Message is %s\n", pipe1writemessage);
      write(pipefds1[1], pipe1writemessage, sizeof(pipe1writemessage));
      read(pipefds2[0], readmessage, sizeof(readmessage));
      printf("In Parent: Reading from pipe 2 – Message is %s\n", readmessage);

      printf("In Parent: Writing to pipe 1 – Message is %s\n", pipe3writemessage);
      write(pipefds1[1], pipe3writemessage, sizeof(pipe3writemessage));
      read(pipefds2[0], readmessage, sizeof(readmessage));
      printf("In Parent: Reading from pipe 2 – Message is %s\n", readmessage);

      printf("In Parent: Writing to pipe 1 – Message is %d\n", varTest);
      write(pipefds1[1], &varTest, sizeof(int));

   }
   else { //child process
      close(pipefds1[1]); // Close the unwanted pipe1 write side
      close(pipefds2[0]); // Close the unwanted pipe2 read side

      read(pipefds1[0], readmessage, sizeof(readmessage));
      printf("In Child: Reading from pipe 1 – Message is %s\n", readmessage);
      printf("In Child: Writing to pipe 2 – Message is %s\n", pipe2writemessage);
      write(pipefds2[1], pipe2writemessage, sizeof(pipe2writemessage));

      read(pipefds1[0], readmessage, sizeof(readmessage));
      printf("In Child: Reading from pipe 1 – Message is %s\n", readmessage);
      printf("In Child: Writing to pipe 2 – Message is %s\n", pipe4writemessage);
      write(pipefds2[1], pipe4writemessage, sizeof(pipe4writemessage));

      read(pipefds1[0], &varTest2, sizeof(int));
      printf("In Child: Reading from pipe 1 – Message is %d\n", varTest2);
   }
   return 0;

}

void startMenu(char *article)
{
	int choixArticle, choixVendeur, choixClient, choixClaissiere;
	printf("Bievenue dans ce programme de simulation de vente en magasin\n");
	printf("Veuillez choisir un article : \n");
	printf("1.Body\t2.Brassiere\t3.Pyjama\n");
	scanf("%d", &choixArticle);
	switch(choixArticle){
		case 1:
			strcpy(article,"Body");
			break;
		case 2:
		strcpy(article,"Brassiere");
			break;
		case 3:
			strcpy(article,"Pyjama");
			break;
	}
	printf("Article dans startMenu : %s\n", article);
}
