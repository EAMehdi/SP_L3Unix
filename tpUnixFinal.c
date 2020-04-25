// Tp Unix/Prog. Systeme  - L3 Informatique - Université Paris Dauphine
// Mehdi EL AYADI & Axel DELERON
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#include <signal.h>

// Menu avec choix des acteurs et de l'article
void startMenu();

// Structure d'un article(nom,prix)
struct Articles {
    char nomArt[20];
    float prixArt;
};

// Structure d'un Sac (propriétaire,article dans le sac)
struct Sacs{
    char owner[50];
    struct Articles monArticle;
};

// Fonction principale Main
int main()
{

    // Variables autour de la structure Articles
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

    // Chaine de caractere, acteurs et article
    char article[20];
    char vendeur[20];
    char client[20];
    char caissiere[20];

    // Variable des tubes
    int pid1, pid2, pid3;
    int pipefds1[2], pipefds2[2];
    int pipefds3[2], pipefds4[2];
    int pipefds5[2], pipefds6[2];
    int returnstatus1, returnstatus2;
    int returnstatus3, returnstatus4;
    int returnstatus5, returnstatus6;

    // Messages du scénario
    char msg1[20] = "'Bonjour'";
    char msg2[20] = "'Bien le bonjour'";
    char msg3[20] = "'Que voulez-vous ?'";
    char msg4[50] = "'Je veux un(e) ";
    char msg5[50] = "'Le prix total est de ";
    char msg6[50] = "'Merci et au revoir'";
    char msg7[50] = "'Quel est le total a payer ?'";
    char msg8[50] = "'Je veux bien un sac'";
    char msgValid[50] = "Fin discussion avec le vendeur";

    // Buffer pour les chaines de caractères
    char readmessage[50];
    char readmessage2[50];

    // Buffer pour les structures Articles et Sacs
    struct Articles readArticle;
    struct Articles readArticle2;
    struct Articles panier;
    struct Sacs readSac;
    struct Sacs readSac2;

    // Lancement du Menu
    startMenu(&article, &vendeur, &client, &caissiere);
    strcat(msg4,article);
    strcat(msg4,"'"); // Adaptation du message 4

    // Articles sélectionné
    for(i=0; i<3; i++){
        if(!strcmp(artListe[i].nomArt,article)){
            strcpy(articleSelected.nomArt,artListe[i].nomArt);
            articleSelected.prixArt=artListe[i].prixArt;
        }
    }
    printf("L'article selectionné:  %s  Prix: %.2f $ \n",articleSelected.nomArt, articleSelected.prixArt);


    // Création du tube au fd 1
    returnstatus1 = pipe(pipefds1);

    if (returnstatus1 == -1) {
        printf("Unable to create pipe 1 \n");
        return 1;
    }

    // Création du tube au fd 2
    returnstatus2 = pipe(pipefds2);

    if (returnstatus2 == -1) {
        printf("Unable to create pipe 2 \n");
        return 1;
    }

    // Création du tube au fd 3
    returnstatus3 = pipe(pipefds3);

    if (returnstatus3 == -1) {
        printf("Unable to create pipe 3 \n");
        return 1;
    }
    // Création du tube au fd 4
    returnstatus4 = pipe(pipefds4);

    if (returnstatus4 == -1) {
        printf("Unable to create pipe 4 \n");
        return 1;
    }

    // Création du tube au fd 5
    returnstatus5 = pipe(pipefds5);

    if (returnstatus1 == -1) {
        printf("Unable to create pipe 5 \n");
        return 1;
    }

    // Création du tube au fd 6
    returnstatus6 = pipe(pipefds6);

    if (returnstatus6 == -1) {
        printf("Unable to create pipe 6 \n");
        return 1;
    }

    // Premier Fork
    pid1 = fork();


    if (pid1 == 0) { // Client (pid == 0 <=> fils)
        close(pipefds1[1]); // Close pipe1 en écriture
        close(pipefds2[0]); // Close pipe2 en lecture

        // Reception du message du Vendeur - Bonjour
        read(pipefds1[0], readmessage2, sizeof(readmessage2));
        printf("In Client: Reading from pipe 1 – %s a dit %s a %s\n", vendeur, readmessage2, client);

        // Le client répond- Bien le Bonjour
        printf("In Client: Writing to pipe 2 – %s dit %s a %s\n", client, msg2, vendeur);
        write(pipefds2[1], msg2, sizeof(msg2));

        // Reception du message - Que voulez-vous ?
        read(pipefds1[0], readmessage2, sizeof(readmessage2));
        printf("In Client: Reading from pipe 1 – %s a dit %s a %s\n", vendeur, readmessage2, client);

        // Le client répond qu'il veut {article sélectionné dans le menu}
        printf("In Client: Writing to pipe 2 – %s dit %s a %s\n", client, msg4, vendeur);
        write(pipefds2[1], msg4, sizeof(msg4));

        // Reception de l'action - le vendeur tend l'article
        read(pipefds1[0], &readArticle2, sizeof(readArticle2));
        printf("In Client: Reading from pipe 1 – %s a tendu le(la) %s a %s \n",vendeur,readArticle2.nomArt, client);

        // Le Client prend l'article et le met dans le panier
        strcpy(panier.nomArt, readArticle2.nomArt);
        panier.prixArt= readArticle2.prixArt;
        printf("In Client: Writing to pipe 2 – %s prend %s a %s\n", client, panier.nomArt,vendeur);
        write(pipefds2[1], &panier, sizeof(panier));
        //wait(NULL);

        // Reception de la validation de fin de discussion avec le vendeur
        read(pipefds1[0], readmessage2, sizeof(readmessage2));
        printf("In Client: Reading from pipe 1 – %s\n", readmessage2);
        printf("--------\n");


        // Le client tend l'article du panier à la caissiere
        printf("In Client: Writing to pipe 2 – %s tend le(la) %s a %s\n", client,panier.nomArt, caissiere);
        write(pipefds2[1], &panier, sizeof(panier));

        // Reception du fait que la caissiere touche l'article
        read(pipefds1[0], &readArticle2, sizeof(readArticle2));
        printf("In Client: Reading from pipe 1 – %s a touche le(la) %s\n", caissiere, readArticle2.nomArt);

        //Le client demande le prix total
        printf("In Client: Writing to pipe 2 – %s dit %s a %s\n", client, msg7, caissiere);
        write(pipefds2[1], msg7, sizeof(msg7));

        // Reception du total à payer
        read(pipefds1[0], readmessage2, sizeof(readmessage2));
        printf("In Client: Reading from pipe 1 – %s a dit %s a %s\n", caissiere, readmessage2 ,client);

        // Le Client paie à la caissiere
        printf("In Client: Writing to pipe 2 – %s paie a %.2f $ a %s\n", client, panier.prixArt, caissiere);
        write(pipefds2[1], &panier, sizeof(panier));

        // Reception du bon encaissement du paiement
        read(pipefds1[0], readmessage2, sizeof(readmessage2));
        printf("In Client: Reading from pipe 1 – %s a encaisse %s $ de %s\n", caissiere, readmessage2,client);

        // Le Client demande un sac
        printf("In Client: Writing to pipe 2  – %s dit %s a %s\n", client, msg8, caissiere);
        write(pipefds2[1], msg8, sizeof(msg8));

        // Reception du sac
        read(pipefds1[0], &readSac2, sizeof(readSac2));
        printf("In Client: Reading from pipe 1 – %s a récupéré le sac\n", readSac2.owner);

        // Le Client dit merci et au revoir
        printf("In Client: Writing to pipe 2 – %s dit %s a %s\n", client, msg6, caissiere);
        write(pipefds2[1], msg6, sizeof(msg6));

        // Reception des remerciements du caissier
        read(pipefds1[0], readmessage2, sizeof(readmessage2));
        printf("In Client: Reading from pipe 1 – %s a dit aussi %s a %s\n", caissiere, readmessage2 ,client);


    }

    else {
        pid2 = fork();
        if (pid2 == 0) { //Vendeur
            close(pipefds3[1]); // Close pipe3 en écriture
            close(pipefds4[0]); // Close pipe4 en lecture

            // Vendeur dit Bonjour au client
            printf("In Vendeur: Writing to pipe 4 – %s dit %s a %s\n", vendeur, msg1, client);
            write(pipefds4[1], msg1, sizeof(msg1));

            //Reception du message - Bien le bonjour
            read(pipefds3[0], readmessage2, sizeof(readmessage2));
            printf("In Vendeur: Reading from pipe 3 – %s a dit %s a %s\n", vendeur, readmessage2, client);

            // Vendeur demande ce que veux au client
            printf("In Vendeur: Writing to pipe 4 – %s dit %s a %s\n", vendeur, msg3, client);
            write(pipefds4[1], msg3, sizeof(msg3));

            // Reception du message - Je veux cette article
            read(pipefds3[0], readmessage2, sizeof(readmessage2));
            printf("In Vendeur: Reading from pipe 3 – %s a dit %s a %s\n", vendeur, readmessage2, client);

            // Le vendeur tend l'article sélectionné
            printf("In Vendeur: Writing to pipe 4 – %s tend le(la) %s a %s\n",vendeur, articleSelected.nomArt, client);
            write(pipefds4[1], &articleSelected, sizeof(articleSelected));

            // Repection de la bonne réception de l'article
            read(pipefds3[0], &readArticle2, sizeof(readArticle2));
            printf("In Vendeur: Reading from pipe 3 – %s a pris %s a %s\n", client, readArticle2.nomArt, vendeur);

            // Fin de la discussion avec le vendeur
            printf("In Vendeur: Writing to pipe 4 – %s\n", msgValid);
            write(pipefds4[1], msgValid, sizeof(msgValid));

            /////////////////
            //     END    //
            ////////////////


        }
        else {
            pid3 = fork();
            if (pid3 == 0) { //Caissiere
                close(pipefds5[1]); // Close pipe5 en écriture
                close(pipefds6[0]); // Close pipe6 en lecture

            // Reception l'article
            read(pipefds5[0], &readArticle2, sizeof(readArticle2));
            printf("In Caissiere: Reading from pipe 5 – %s a tendu le(la) %s a %s\n", client, readArticle2.nomArt, caissiere);

            // La caissiere touche l'article
            printf("In Caissiere: Writing to pipe 6 – %s touche %s\n",caissiere, readArticle2.nomArt);
            write(pipefds6[1], &readArticle2, sizeof(readArticle2));
            //
            // Reception de la demande du total a payer
            read(pipefds5[0], readmessage2, sizeof(readmessage2));
            printf("In Caissiere: Reading from pipe 5 – %s a dit %s a %s\n", client, readmessage2, caissiere);

            // La caissiere annonce le total a payer
            char buf[10];
            gcvt(readArticle2.prixArt,4,buf);
            strcat(msg5,buf);
            strcat(msg5, "$'");
            printf("In Caissiere: Writing to pipe 6 – %s dit %s a %s\n",caissiere, msg5 ,client);
            write(pipefds6[1], msg5, sizeof(msg5));

            // Reception du paiement de la cliente
            read(pipefds5[0],  &readArticle2, sizeof(readArticle2));
            printf("In Caissiere: Reading from pipe 5 – %s a payé %.2f $ a %s\n", client, readArticle2.prixArt, caissiere);

            // La caissiere encaisse le paiement de la cliente
            printf("In Caissiere: Writing to pipe 6 – %s encaisse les %s$ a %s\n",caissiere, buf, client);
            write(pipefds6[1], buf, sizeof(buf));

            // Reception de la demande d'un sac du client
            read(pipefds5[0], readmessage2, sizeof(readmessage2));
            printf("In Caissiere: Reading from pipe 5 – %s a dit %s a %s\n", client, readmessage2, caissiere);

            // La caissiere met l'article et le ticket de caisse dans le Sac
            strcpy(sac.owner,client);
            sac.monArticle = readArticle2;
            printf("In Caissiere: Writing to pipe 6 – %s met l'article et le ticket de caisse dans le sac de %s\n", caissiere, sac.owner);
            write(pipefds6[1], &sac, sizeof(sac));

            // Reception des remerciements du client
            read(pipefds5[0], readmessage2, sizeof(readmessage2));
            printf("In Caissiere: Reading from pipe 5 – %s a dit %s a %s\n", client, readmessage2, caissiere);

            // La caissiere remercie le client
            printf("In Caissiere: Writing to pipe 6 – %s dit %s a %s\n",caissiere,msg6 ,client);
            write(pipefds6[1], msg6, sizeof(msg6));

            }
            else { //Parent
                close(pipefds1[0]); // Close pipe1 en lecture
                close(pipefds2[1]); // Close pipe2 en écriture
                close(pipefds3[0]); // Close pipe3 en lecture
                close(pipefds4[1]); // Close pipe4 en écriture
                close(pipefds5[0]); // Close pipe5 en lecture
                close(pipefds6[1]); // Close pipe6 en écriture

                read(pipefds4[0], readmessage, sizeof(readmessage));
                write(pipefds1[1], readmessage, sizeof(readmessage));

                read(pipefds2[0], readmessage, sizeof(readmessage));
                write(pipefds3[1], readmessage, sizeof(readmessage));

                read(pipefds4[0], readmessage, sizeof(readmessage));
                write(pipefds1[1], readmessage, sizeof(readmessage));

                read(pipefds2[0], readmessage, sizeof(readmessage));
                write(pipefds3[1], readmessage, sizeof(readmessage));

                read(pipefds4[0], &readArticle, sizeof(readArticle));
                write(pipefds1[1], &readArticle, sizeof(readArticle));

                read(pipefds2[0], &readArticle, sizeof(readArticle));
                write(pipefds3[1], &readArticle, sizeof(readArticle));

                read(pipefds4[0], readmessage, sizeof(readmessage));
                write(pipefds1[1], readmessage, sizeof(readmessage));

                // Client tend article a la caissiere
                read(pipefds2[0], &readArticle, sizeof(readArticle));
                write(pipefds5[1], &readArticle, sizeof(readArticle));

                //Caissiere touche article
                read(pipefds6[0], &readArticle, sizeof(readArticle));
                write(pipefds1[1], &readArticle, sizeof(readArticle));

                // Client demande prix total
                read(pipefds2[0], readmessage, sizeof(readmessage));
                write(pipefds5[1], readmessage, sizeof(readmessage));

                // Caissiere annonce prix
                read(pipefds6[0], readmessage, sizeof(readmessage));
                write(pipefds1[1], readmessage, sizeof(readmessage));

                //Annonce du prix à payer
                read(pipefds2[0], &readArticle, sizeof(readArticle));
                write(pipefds5[1], &readArticle, sizeof(readArticle));

                // Caissiere encaisse client
                read(pipefds6[0], readmessage, sizeof(readmessage));
                write(pipefds1[1], readmessage, sizeof(readmessage));

                // Client demande un sac
                read(pipefds2[0], readmessage, sizeof(readmessage));
                write(pipefds5[1], readmessage, sizeof(readmessage));

                // Reception du sac
                read(pipefds6[0], &readSac, sizeof(readSac));
                write(pipefds1[1], &readSac, sizeof(readSac));

                // Client dit merci et au revoir
                read(pipefds2[0], readmessage, sizeof(readmessage));
                write(pipefds5[1], readmessage, sizeof(readmessage));

                // Caissiere dit merci et au revoir
                read(pipefds6[0], readmessage, sizeof(readmessage));
                write(pipefds1[1], readmessage, sizeof(readmessage));

            }
        }
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
