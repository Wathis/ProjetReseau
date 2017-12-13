#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main () {

	int socketFd = socket(AF_INET, SOCK_STREAM,0);
	struct sockaddr_in serveur;
	serveur.sin_family = AF_INET;
	serveur.sin_port = htons(7979);
	serveur.sin_addr.s_addr = inet_addr("192.168.0.102");
	if (bind(socketFd,(struct sockaddr *) &serveur, sizeof(serveur)) < 0) {
		perror("Erreur dans le bind");
	} else {
		printf("Binding ... \n");
	}
	if (listen(socketFd,1) < 0 ) {
		perror("Erreur dans le listen");
	} else {
		printf("Ecoute ... \n");
   }

	struct sockaddr_in client;
	client.sin_family = AF_INET;
	
	unsigned int add_client = sizeof(client);

	printf("Attente d'un client ...\n");
	char plaqueImmatriculation[8];
	char categorie[2];
	int duree = 0;
	int socketClient = -1;
	while (1) {
		socketClient = accept(socketFd, (struct sockaddr *)&client, &add_client);
		printf("Tentative de connexion ...");
		if (socketClient > 0 ) {
			printf("-------------------------------------\n");
			printf("/!\\ Demande connexion client accepté\n");
			printf("-------------------------------------\n");
			read(socketClient,plaqueImmatriculation,8);
			read(socketClient,categorie,2);
			read(socketClient,&duree,sizeof(int));
			printf("/INFO\\ Demande de la plaque : %s categorie : %s pour duree : %d\n\n",plaqueImmatriculation,categorie, duree);
		} else {
			perror("Erreur de accept\n");	
		}
	}
	close(socketFd);
	
}
