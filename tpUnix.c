#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <signal.h>

void startMenu();

struct Articles {
	char nomArt[20];
	float prixArt;
};

struct Sacs{
	char owner[50];
	struct Articles monArticle;
};

int main() {
	//char *articleList[] = {"body","brassiere","pyjama"};
	struct Articles body;
	struct Articles brassiere;
	struct Articles pyjama;
	struct Sacs sac;
	int i;

	strcpy(body.nomArt, "Body");
	body.prixArt= 19.95;
	strcpy(brassiere.nomArt, "Brassiere");
	brassiere.prixArt= 14.95;
	strcpy(pyjama.nomArt, "Pyjama");
	pyjama.prixArt= 24.95;

	struct Articles artListe[3] = {body, brassiere, pyjama};
	struct Articles articleSelected;

	char article[20];
	char vendeur[20];
	char client[20];
	char caissiere[20];

	int pipefds1[2], pipefds2[2];
	int pipefds3[2], pipefds4[2];
	int returnstatus1, returnstatus2;
	int returnstatus3, returnstatus4;
	int pid;
	char pipe1writemessage[20] = "'Bonjour'";
	char pipe2writemessage[20] = "'Bien le bonjour'";
	char pipe3writemessage[20] = "'Que voulez-vous ?'";
	char pipe4writemessage[50] = "'Je veux un(e) ";
	char pipe5writemessage[50] = "'Le prix total est de ";
	char pipe6writemessage[50] = "'Merci et au revoir'";
	char pipe7writemessage[50] = "'Quel est le total a payer ?'";
	char pipe8writemessage[50] = "'Je veux bien un sac'";
	char readmessage[50];

	struct Articles readArticle;
	struct Articles panier;
	struct Sacs readSacs;

	startMenu(&article, &vendeur, &client, &caissiere);
	printf("Vendeur : %s\n",vendeur);
	printf("Client : %s\n",client);
	printf("Caissiere : %s\n",caissiere);


	for(i=0; i<3; i++){
		if(!strcmp(artListe[i].nomArt,article)){
			strcpy(articleSelected.nomArt,artListe[i].nomArt);
			articleSelected.prixArt=artListe[i].prixArt;
		}
	}
	strcat(pipe4writemessage,articleSelected.nomArt);
	strcat(pipe4writemessage,"'");
	printf("\nL'article selectionné:  %s  \nPrix: %.2f $ \n",articleSelected.nomArt, articleSelected.prixArt);

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


	// FORK CLIENT-VENDEUR
	printf("\n------------ Conversation entre la cliente et le vendeur ------------\n");
	pid = fork();

	if (pid != 0)  {  // Parent process --> Client
		//printf("Father PID = %d %d\n", getpid(), getppid());
		close(pipefds1[1]); // Close the unwanted pipe1 write side
		close(pipefds2[0]); // Close the unwanted pipe2 read side

		// Reception du message du Vendeur - Bonjour
		read(pipefds1[0], readmessage, sizeof(readmessage));
		printf("In Parent: Reading from pipe 1 – %s a dit %s a %s\n", vendeur, readmessage, client);

		// Le client répond- Bien le Bonjour
		printf("In Parent: Writing to pipe 2 – %s dit %s a %s\n", client, pipe2writemessage, vendeur);
		write(pipefds2[1], pipe2writemessage, sizeof(pipe2writemessage));

		// Reception du message - Que voulez-vous ?
		read(pipefds1[0], readmessage, sizeof(readmessage));
		printf("In Parent: Reading from pipe 1 – %s a dit %s a %s\n", vendeur, readmessage, client);

		// Le client répond qu'il veut {article sélectionné dans le menu}
		printf("In Parent: Writing to pipe 2 – %s dit %s a %s\n", client, pipe4writemessage,vendeur);
		write(pipefds2[1], pipe4writemessage, sizeof(pipe4writemessage));

		// Reception de l'action - le vendeur tend l'article
		read(pipefds1[0], &readArticle, sizeof(readArticle));
		printf("In Parent: Reading from pipe 1 – %s a tendu le(la) %s a %s \n",vendeur,readArticle.nomArt, client);

		// Le Client prend l'article et le met dans le panier
		strcpy(panier.nomArt, readArticle.nomArt);
		panier.prixArt= readArticle.prixArt;
		printf("In Parent: Writing to pipe 2 – %s prend %s a %s\n", client, panier.nomArt,vendeur);
		write(pipefds2[1], &panier, sizeof(panier));
		wait(NULL);

	}
	else { //child process --> Vendeur
		//printf("First Son PID = %d %d\n", getpid(), getppid());
		close(pipefds1[0]); // Close the unwanted pipe1 read side
		close(pipefds2[1]); // Close the unwanted pipe2 write side

		// Vendeur dit Bonjour au client
		printf("In Child: Writing to pipe 1 – %s dit %s a %s\n", vendeur, pipe1writemessage, client);
		write(pipefds1[1], pipe1writemessage, sizeof(pipe1writemessage));

		//Reception du message - Bien le bonjour
		read(pipefds2[0], readmessage, sizeof(readmessage));
		printf("In Child: Reading from pipe 2 – %s a dit %s a %s\n", client, readmessage, vendeur);

		// Vendeur demande ce que veux au client
		printf("In Child: Writing to pipe 1 – %s dit %s a %s\n", vendeur, pipe3writemessage, client);
		write(pipefds1[1], pipe3writemessage, sizeof(pipe3writemessage));

		// Reception du message - Je veux cette article
		read(pipefds2[0], readmessage, sizeof(readmessage));
		printf("In Child: Reading from pipe 2 – %s a dit %s a %s\n", client, readmessage, vendeur);

		// Le vendeur tend l'article sélectionné
		printf("In Child: Writing to pipe 1 – %s tend le(la) %s a %s\n",vendeur, articleSelected.nomArt, client);
		write(pipefds1[1], &articleSelected, sizeof(articleSelected));

		// Repection de la bonne réception de l'article
		read(pipefds2[0], &readArticle, sizeof(readArticle));
		printf("In Child: Reading from pipe 2 – %s a pris %s a %s\n", client, readArticle.nomArt, vendeur);

		// On ferme le processus du vendeur
		kill(getpid(),SIGTERM);
	}

	returnstatus3 = pipe(pipefds3);

	if (returnstatus3 == -1) {
		printf("Unable to create pipe 3 \n");
		return 1;
	}
	returnstatus4 = pipe(pipefds4);

	if (returnstatus4 == -1) {
		printf("Unable to create pipe 4 \n");
		return 1;
	}

	printf("\n------------ Conversation entre la cliente et la caissiere ------------\n");
	pid= fork();
	if (pid != 0)  {  // Parent process --> Client
		close(pipefds3[1]); // Close the unwanted pipe3 write side
		close(pipefds4[0]); // Close the unwanted pipe4 read side
		//printf("Father PID = %d %d\n", getpid(), getppid());
		// Le client tend l'article du panier à la caissiere
		printf("In Parent: Writing to pipe 4 – %s tend le(la) %s a %s\n", client,panier.nomArt, caissiere);
		write(pipefds4[1], &panier, sizeof(panier));

		// Reception du fait que la caissiere touche l'article
		read(pipefds3[0], &readArticle, sizeof(readArticle));
		printf("In Parent: Reading from pipe 3 – %s a touche le(la) %s\n", caissiere, readArticle.nomArt);

		//Le client demande le prix total
		printf("In Parent: Writing to pipe 4 – %s dit %s a %s\n", client,pipe7writemessage, caissiere);
		write(pipefds4[1], pipe7writemessage, sizeof(pipe7writemessage));

		// Reception du total à payer
		read(pipefds3[0], readmessage, sizeof(readmessage));
		printf("In Parent: Reading from pipe 3 – %s a dit %s a %s\n", caissiere, readmessage,client);

		// Le Client paie à la caissiere
		printf("In Parent: Writing to pipe 4 – %s paie a %.2f a %s\n", client,readArticle.prixArt, caissiere);
		write(pipefds4[1], &panier, sizeof(panier));

		// Reception du bon encaissement du paiement
		read(pipefds3[0], readmessage, sizeof(readmessage));
		printf("In Parent: Reading from pipe 3 – %s a encaisse %s a %s\n", caissiere, readmessage,client);

		// Le Client demande un sac
		printf("In Parent: Writing to pipe 4  – %s dit %s a %s\n", client,pipe8writemessage, caissiere);
		write(pipefds4[1], pipe8writemessage, sizeof(pipe8writemessage));

		// Reception du sac
		read(pipefds3[0], &readSacs, sizeof(readSacs));
		printf("In Parent: Reading from pipe 3 – %s a récupéré le sac\n", client);

		// Le Client dit merci et au revoir
		printf("In Parent: Writing to pipe 4  – %s dit %s a %s\n", client,pipe6writemessage, caissiere);
		write(pipefds4[1], pipe6writemessage, sizeof(pipe6writemessage));

		// Reception du bon encaissement du paiement
		read(pipefds3[0], readmessage, sizeof(readmessage));
		printf("In Parent: Reading from pipe 3 – %s a dit %s a %s\n", caissiere, readmessage,client);

		wait(NULL);

	}
	else { //child process --> Caissiere
		//printf("Second Son PID = %d %d\n", getpid(), getppid());
		close(pipefds3[0]); // Close the unwanted pipe3 read side
		close(pipefds4[1]); // Close the unwanted pipe4 write side

		// Repection de l'article
		read(pipefds4[0], &readArticle, sizeof(readArticle));
		printf("In Child: Reading from pipe 4 – %s a tendu le(la) %s a %s\n", client, readArticle.nomArt, caissiere);

		// La caissiere touche l'article
		printf("In Child: Writing to pipe 3 – %s touche %s\n",caissiere, readArticle.nomArt);
		write(pipefds3[1], &readArticle, sizeof(readArticle));

		// Reception de la demande du total a payer
		read(pipefds4[0], readmessage, sizeof(readmessage));
		printf("In Child: Reading from pipe 4 – %s a dit %s a %s\n", client, readmessage, caissiere);

		// La caissiere annonce le total a payer
		char buf[10];
		gcvt(readArticle.prixArt,4,buf);
		strcat(pipe5writemessage,buf);
		strcat(pipe5writemessage, "$'");
		printf("In Child: Writing to pipe 3 – %s dit %s a %s\n",caissiere,pipe5writemessage ,client);
		write(pipefds3[1], pipe5writemessage, sizeof(pipe5writemessage));

		// Reception du paiement de la cliente
		read(pipefds4[0],  &readArticle, sizeof(readArticle));
		printf("In Child: Reading from pipe 4 – %s a payé %.2f $ a %s\n", client, readArticle.prixArt, caissiere);

		// La caissiere encaisse le paiement de la cliente
		printf("In Child: Writing to pipe 3 – %s encaisse les %s$ a %s\n",caissiere,buf,client);
		write(pipefds3[1], buf, sizeof(buf));

		// Reception de la demande d'un sac du client
		read(pipefds4[0], readmessage, sizeof(readmessage));
		printf("In Child: Reading from pipe 4 – %s a dit %s a %s\n", client, readmessage, caissiere);

		// La caissiere met l'article et le ticket de caisse dans le Sac
		strcat(sac.owner,client);
		sac.monArticle=readArticle;
		printf("In Child: Writing to pipe 3 – %s met l'article et le ticket de caisse dans le sac de %s\n",caissiere,sac.owner);
		write(pipefds3[1], &sac, sizeof(sac));

		// Reception des remerciements du client
		read(pipefds4[0], readmessage, sizeof(readmessage));
		printf("In Child: Reading from pipe 4 – %s a dit %s a %s\n", client, readmessage, caissiere);

		// La caissiere remercie le client
		printf("In Child: Writing to pipe 3 – %s dit %s a %s\n",caissiere,pipe6writemessage ,client);
		write(pipefds3[1], pipe6writemessage, sizeof(pipe6writemessage));
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

	printf("Veuillez choisir un client: \n");
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
