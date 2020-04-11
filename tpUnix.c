#include<stdio.h>
#include<unistd.h>
#include<string.h>
void startMenu();

struct Articles {
	char nomArt[20];
	float prixArt;
};

int main() {
	//char *articleList[] = {"body","brassiere","pyjama"};
	struct Articles body;
	struct Articles brassiere;
	struct Articles pyjama;
	int i;

	strcpy(body.nomArt, "Body");
	body.prixArt= 19.95;

	strcpy(brassiere.nomArt, "Brassiere");
	brassiere.prixArt= 14.95;

	strcpy(pyjama.nomArt, "Pyjama");
	pyjama.prixArt= 24.95;

	struct Articles artListe[3] = {body, brassiere, pyjama};
	struct Articles articleSelected;


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
	char pipe1writemessage[20] = "'Bonjour'";
	char pipe2writemessage[20] = "'Bien le bonjour'";
	char pipe3writemessage[20] = "'Que voulez-vous ?'";
	char pipe4writemessage[50] = "'Je veux un(e) ";
	char readmessage[50];
	struct Articles readArticle;
	struct Articles panier;

	int varTest = 99;
	int varTest2;

	startMenu(&article, &vendeur, &client, &caissiere);
	printf("Article dans main : %s\n",article);
	printf("Article dans main : %s\n",vendeur);
	printf("Article dans main : %s\n",client);
	printf("Article dans main : %s\n",caissiere);
	strcat(pipe4writemessage,article);
	strcat(pipe4writemessage,"'");

	for(i=0; i<3; i++){
		if(!strcmp(artListe[i].nomArt,article)){
			strcpy(articleSelected.nomArt,artListe[i].nomArt);
			articleSelected.prixArt=artListe[i].prixArt;
		}
	}
	printf("VOICI L'ARTICLE SELECTED:  %s avec un prix de %f \n",articleSelected.nomArt, articleSelected.prixArt);

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


	// Processus avec le vendeur
	pid = fork();

	if (pid != 0)  {  // Parent process --> Client
		close(pipefds1[1]); // Close the unwanted pipe1 write side
		close(pipefds2[0]); // Close the unwanted pipe2 read side

		read(pipefds1[0], readmessage, sizeof(readmessage));
		printf("In Parent: Reading from pipe 1 – %s a dit %s a %s\n", vendeur, readmessage, client);
		printf("In Parent: Writing to pipe 2 – %s dit %s a %s\n", client, pipe2writemessage, vendeur);
		write(pipefds2[1], pipe2writemessage, sizeof(pipe2writemessage));

		read(pipefds1[0], readmessage, sizeof(readmessage));
		printf("In Parent: Reading from pipe 1 – %s a dit %s a %s\n", vendeur, readmessage, client);

		printf("In Parent: Writing to pipe 2 – %s dit %s a %s\n", client, pipe4writemessage,vendeur);
		write(pipefds2[1], pipe4writemessage, sizeof(pipe4writemessage));

		read(pipefds1[0], &readArticle, sizeof(readArticle));
		printf("In Parent: Reading from pipe 1 – %s a tendu %s a %s \n",vendeur,readArticle.nomArt, client);

		// Client prend l'article
		strcpy(panier.nomArt, readArticle.nomArt);
		panier.prixArt= readArticle.prixArt;
		printf("In Parent: Writing to pipe 2 – %s prend %s a %s\n", client, panier.nomArt,vendeur);
		write(pipefds2[1], &panier, sizeof(panier));
		wait(NULL);

	}
	else { //child process --> Vendeur
		close(pipefds1[0]); // Close the unwanted pipe1 read side
		close(pipefds2[1]); // Close the unwanted pipe2 write side

		printf("In Child: Writing to pipe 1 – %s dit %s a %s\n", vendeur, pipe1writemessage, client);
		write(pipefds1[1], pipe1writemessage, sizeof(pipe1writemessage));
		read(pipefds2[0], readmessage, sizeof(readmessage));
		printf("In Child: Reading from pipe 2 – %s a dit %s a %s\n", client, readmessage, vendeur);

		printf("In Child: Writing to pipe 1 – %s dit %s a %s\n", vendeur, pipe3writemessage, client);
		write(pipefds1[1], pipe3writemessage, sizeof(pipe3writemessage));

		read(pipefds2[0], readmessage, sizeof(readmessage));
		printf("In Child: Reading from pipe 2 – %s a dit %s a %s\n", client, readmessage, vendeur);


		printf("In Child: Writing to pipe 1 – %s tend %s a %s\n",vendeur, articleSelected.nomArt, client);
		write(pipefds1[1], &articleSelected, sizeof(articleSelected));

		read(pipefds2[0], &readArticle, sizeof(readArticle));
		printf("In Child: Reading from pipe 2 – %s a pris %s a %s\n", client, readArticle.nomArt, vendeur);

	}
	return 0;

}



void startMenu(char *article, char *vendeur, char *client, char *caissiere)
{
	int choixArticle, choixVendeur, choixClient, choixClaissiere;
	printf("Bienvenue dans ce programme de simulation de vente en magasin\n");
	printf("Veuillez choisir un article : \n");
	printf("1.Body   2.Brassiere   3.Pyjama\n");
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

	printf("Veuillez choisir un vendeur : \n");
	printf("1.Pierre   2.Paul   3.Jacques\n");
	scanf("%d", &choixVendeur);
	switch(choixArticle){
		case 1:
		strcpy(vendeur,"Pierre");
		break;
		case 2:
		strcpy(vendeur,"Paul");
		break;
		case 3:
		strcpy(vendeur,"Jacques");
		break;
	}

	printf("Veuillez choisir une cliente : \n");
	printf("1.Chloe   2.Elise   3.Lea\n");
	scanf("%d", &choixClient);
	switch(choixClient){
		case 1:
		strcpy(client,"Chloe");
		break;
		case 2:
		strcpy(client,"Elise");
		break;
		case 3:
		strcpy(client,"Lea");
		break;
	}

	printf("Veuillez choisir une caissiere : \n");
	printf("1.Lilou   2.Laura   3.Nadia\n");
	scanf("%d", &choixClaissiere);
	switch(choixClaissiere){
		case 1:
		strcpy(caissiere,"Lilou");
		break;
		case 2:
		strcpy(caissiere,"Laura");
		break;
		case 3:
		strcpy(caissiere,"Nadia");
		break;
	}
}
