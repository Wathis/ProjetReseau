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
#include <time.h>

struct Voiture {
	//Plaque d'immatriculation du vehicule
	char plaque[8];
	//Time stamp de l'heure d'arrivée du vehicule
	int timeStampArrivee;
	//Categorie 
	char categorie;
};

#define NBR_MAX_PLACE 5
#define SERVEUR_PORT 7777

#include "CONFIG_SERVEUR.h"

//Pour stocker le nombre de forfaits 
int NBR_FORFAITS = sizeof(prixForfaits) / sizeof(prixForfaits[0]);









