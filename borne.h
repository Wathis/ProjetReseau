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

#include "CONFIG_BORNE.h"

//Permet d'afficher le menu principale de l'application
int menuPrincipal();

//Permet d'afficher un message entouré par des "-"
void afficherMessage(char message[]);

void viderBuffer();

//Permet de donner le prix a payer en fonction des parametres d'entrée
float price(float dureeMaxForfait, float dureeVoitureHeures, float prixForfait, float prixHorsForfait);

//Permet d'afficher "-" number fois
void printLines(int number);

//Premier protocole correspondant à une proposition de contrat de stationnement pour une voiture donnée
void premierProtocole(int socketServeur,char ipServeur[],char plaqueImmatriculation[],float dureeSouhaiteeHeures,char categorie);

//Deuxieme protocole correspondant à un état du coût de stationnement pour une voiture donnée
void deuxiemeProtocole(int socketServeur,char ipServeur[],char plaqueImmatriculation[]);