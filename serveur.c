#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "serveur.h"

int main (int argc, char *argv[]) {

	int socketFd = socket(AF_INET, SOCK_STREAM,0);
	struct sockaddr_in serveur;
	serveur.sin_family = AF_INET;
	serveur.sin_port = htons(SERVEUR_PORT);
	//On recupere l'addresse du serveur donnée en parametres
	if (argc > 1) { 
		serveur.sin_addr.s_addr = inet_addr(argv[1]);
	} else {
		perror("Vous devez entrer l'addresse du serveur");
		exit(1);
	}
	if (bind(socketFd,(struct sockaddr *) &serveur, sizeof(serveur)) < 0) {
		perror("Erreur dans le bind");
	} else {
		printf("Binding ... \n");
	}
	if (listen(socketFd,1) < 0 ) {
		perror("Erreur dans le listen");
	} else {
		printf("Ecoute sur %d... \n",SERVEUR_PORT);
   }

	struct sockaddr_in client;
	client.sin_family = AF_INET;
	unsigned int add_client = sizeof(client);

	//index 0 correspond au forfait A, 1 correspond forfait B
	float prixForfaits[] = {2,3};
	float prixHorsForfaits[] = {5,8};
	int nbPlacesParCategories[] = {2,2};

	int NBR_FORFAITS = sizeof(prixForfaits) / sizeof(prixForfaits[0]);

	char plaqueImmatriculation[8];
	char categorie;
	int duree ;
	int socketClient;
	float prixForfait;
	float prixHorsForfait;

	while (1) {
		socketClient = accept(socketFd, (struct sockaddr *)&client, &add_client);
		printf("Tentative de connexion ...\n");
		if (socketClient > 0 ) {
			if (fork() == 0) {
				printf("-------------------------------------\n");
				printf("/!\\ Demande connexion client accepté\n");
				printf("-------------------------------------\n");
				read(socketClient,&categorie,1);
				read(socketClient,plaqueImmatriculation,8);
				read(socketClient,&duree,sizeof(int));
				printf("/INFO\\ Demande de la plaque : %s categorie : %c pour duree : %d\n\n",plaqueImmatriculation,categorie, duree);
				if (categorie - 'A' > NBR_FORFAITS) {
					prixForfait = prixForfaits[categorie - 'A'];
					prixHorsForfait = prixHorsForfaits[categorie - 'A'];
					if (nbPlacesParCategories[categorie - 'A'] > 0) {
						write(socketClient,"O",1);
						write(socketClient,argv[1],8);
						write(socketClient,&duree,sizeof(int));
					} else {
						//Il n'y a plus de place pour la categorie demandée
						write(socketClient,"N",1);
					}
				}
				close(socketClient);
			}
		} else {
			perror("Erreur de accept\n");	
		}
	}
	close(socketFd);
	
}
