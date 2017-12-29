#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <time.h>

struct Voiture {
	//Plaque d'immatriculation du vehicule
	char plaque[8];
	//Time stamp de l'heure d'arrivée du vehicule
	long int timeStampArrivee;
	//Categorie 
	char categorie;
};

#define NBR_MAX_PLACE 5

#include "CONFIG_SERVEUR.h"

//Pour stocker le nombre de forfaits 
int NBR_FORFAITS = sizeof(prixForfaits) / sizeof(prixForfaits[0]);

//Permet de mettre des voitures par default dans le parking
void mettreVoitureDansParking();

//Permet savoir ce que le client a choisi comme type de protocole a suivre
void choisirUnProtocole(int socketClient,struct sockaddr_in serveur);

// Premier protocole correspondant à une proposition de contrat de stationnement pour une voiture donnée
void premierProtocole(int socketClient,struct sockaddr_in serveur);

//Deuxieme protocole correspondant à un état du coût de stationnement pour une voiture donnée
void deuxiemeProtocole(int socketClient,struct sockaddr_in serveur);

//Permet de savoir si une voiture est présente ou non dans le parking
// La fonction renvoie l'index ou se trouve la voiture, 
// Si l'index vaut -1, c'est que la voiture n'est pas garé dans le parking
int rechercheVoitureParking(char plaqueImmatriculation[]);







