#include "serveur.h"


int main (int argc, char *argv[]) {

	int socketFd = socket(AF_INET, SOCK_STREAM,0);
	struct sockaddr_in serveur;
	serveur.sin_family = AF_INET;
	serveur.sin_port = htons(SERVEUR_PORT);
	//On recupere l'addresse du serveur donnée en parametres
	if (argc > 1) { 
		//On stocke l'addresse du serveur
    	inet_aton(argv[1], &serveur.sin_addr);
	} else {
		perror("Vous devez entrer l'addresse du serveur");
		exit(1);
	}
	//On regarde si il n'y a pas d'erreur dans le bind
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

	//On stocke une voiture de categorie A
	strcpy(voituresPresentes[0].plaque,"AAABBCC\0");
	voituresPresentes[0].timeStampArrivee = time(0) - 1000; // Timestamp actuel
	voituresPresentes[0].categorie = 'A';

	char plaqueImmatriculation[8];
	char categorie;
	int menu;
	int duree ;
	int socketClient;
	float dureeMaxForfait;
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
				read(socketClient,&menu,sizeof(int));
				//Si menu == 1 c'est que la borne veut une proposition de contrat de stationnement pour une voiture donnée 
				if (menu == 1) {
					read(socketClient,&categorie,1);
					read(socketClient,plaqueImmatriculation,8);
					read(socketClient,&duree,sizeof(int));
					printf("/INFO\\ Demande de la plaque : %s categorie : %c pour duree : %d\n\n",plaqueImmatriculation,categorie, duree);
					if (categorie - 'A' < NBR_FORFAITS) {
						dureeMaxForfait = dureeMaxForfaits[categorie - 'A'];
						prixForfait = prixForfaits[categorie - 'A'];
						prixHorsForfait = prixHorsForfaits[categorie - 'A'];
						printf("Envoie de %f  %f \n",prixForfait,prixHorsForfait);
						//On envoie la reponse au client, avec le prix forfait et prix hors forfait correspondant a sa catégorie de vehicule
						if (nbPlacesLibresParCategories[categorie - 'A'] > 0) {
							// "O" permet de dire que oui on a de la place
							write(socketClient,"O",1);
							// On donne le nom du serveur au client ( son addresse au format integer ) 
							write(socketClient,&serveur.sin_addr,sizeof(serveur.sin_addr));
							//On fait la suite du protocole
							write(socketClient,&dureeMaxForfait,sizeof(float));
							write(socketClient,&prixForfait,sizeof(float));
							write(socketClient,&prixHorsForfait,sizeof(float));
						} else {
							//Il n'y a plus de place pour la categorie demandée
							write(socketClient,"N",1);
						}
					} else {
						//Le forfait n'existe pas 
						write(socketClient,"E",1);
					}
				} else {
					//Sinon c'est qu'elle veut connaitre un état du coût de stationnement pour une voiture donnée
					read(socketClient,plaqueImmatriculation,8);

					//On parcours les voitures presentes pour voir si celle demandée est presente
					int i = 0;
					for (i = 0 ; i < NBR_MAX_PLACE ; i++) {
						if (strcmp(voituresPresentes[i].plaque,plaqueImmatriculation) == 0) {
							break;
						}
					}
					//Si i == NBR_MAX_PLACE - 1 C'est que l'on a pas trouvé la voiture
					if (i == NBR_MAX_PLACE) {
						//On renvoie 'N' au client pour dire que on ne l'a pas trouvé
						write(socketClient,"N",sizeof(char));
					} else { //Sinon c'est qu'elle est bien dans le parking, on peut alors renvoyer les informations au client
						//On renvoie 'O' au client pour dire que l'on a sa voiture
						write(socketClient,"O",sizeof(char));

						//On va chercher les differentes informations en fonctions de la categorie du vehicule
						categorie = voituresPresentes[i].categorie;
						dureeMaxForfait = dureeMaxForfaits[categorie - 'A'];
						prixForfait = prixForfaits[categorie - 'A'];
						prixHorsForfait = prixHorsForfaits[categorie - 'A'];

						// On donne le nom du serveur au client ( son addresse ) 
						write(socketClient,&serveur.sin_addr,sizeof(serveur.sin_addr));

						//On extrait la durée a laquelle est resté la voiture dans le parking
						long int dureeVoiture = time(0) - voituresPresentes[i].timeStampArrivee;
						float dureeVoitureHeures = (float) dureeVoiture / (float) 3600; //On repasse le timestamp secondes en heures
						//On fait la suite du protocole
						write(socketClient,&dureeVoitureHeures,sizeof(float));
						write(socketClient,&dureeMaxForfait,sizeof(float));
						write(socketClient,&prixForfait,sizeof(float));
						write(socketClient,&prixHorsForfait,sizeof(float));
					}
				}
				close(socketClient);
				printf("Communication fermée avec le client\n");
			}
		} else {
			perror("Erreur de accept\n");	
		}
	}
	close(socketFd);
	
}
