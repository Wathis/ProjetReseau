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

void viderBuffer();

//Permet de donner le prix a payer en fonction des parametres d'entrée
float price(float dureeMaxForfait, float dureeVoitureHeures, float prixForfait, float prixHorsForfait);

//Permet d'afficher "-" number fois
void printLines(int number);