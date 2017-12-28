#include "borne.h"

void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

float price(float dureeMaxForfait, float dureeVoitureHeures, float prixForfait, float prixHorsForfait){
	float price;
	if (dureeVoitureHeures - dureeMaxForfait < 0)
	{
		price = dureeVoitureHeures*prixForfait;
	} else {
		price = dureeMaxForfait*prixForfait + (dureeVoitureHeures - dureeMaxForfait)*prixHorsForfait;
	}	
	return price;
}

int main (int argc, char *argv[]) {
	int connectionInfo; 
	struct sockaddr_in serveur;
	serveur.sin_family = AF_INET;
	serveur.sin_port = htons(7777);	

	serveur.sin_addr.s_addr = inet_addr(argv[1]);
	int s = socket(AF_INET,SOCK_STREAM,0);
	char plaqueImmatriculation[8];
	char categorie = 'A';
	int duree = 12;
	char repon;
	


	// strcpy(plaqueImmatriculation,"AAABBCC\0");
	// categorie = 'B';
	// duree = 2;
	
	// test de la validité de la socket
	if (s < 0) {
		perror("Erreur de socket");
	} else {
		// connection au serveur
		if (connect(s,(struct sockaddr*) &serveur,sizeof(serveur)) < 0) {
			perror("Erreur de connexion");
		}else {
			printf("Connecté au serveur\n");

			do {
				printf("1 pour tarif\n2 pour payer\n");
				scanf("%c", &repon);
				
			} while (repon != '1' &&  repon != '2');

			
			int reponInteger = repon - '0';	

			//On envoie au serveur si on veut payer ou voir les tarifs
			write(s, &reponInteger,sizeof(int));

			char reponse;
			struct sockaddr_in serveurReponse;
			char *ipServeurReponse;
			float dureeMaxForfait;
			float prixForfait;
			float prixHorsForfait;

			if(reponInteger == 1) {
				viderBuffer();
				//On saisi les informations du client 
				printf("catégorie : ");
				scanf("%c", &categorie);
				viderBuffer();
				printf("plaque d'immatriculation : ");
				scanf("%s", plaqueImmatriculation);	
				viderBuffer();
				printf("duree : ");
				scanf("%d", &duree);
				viderBuffer();
				
				//On envoie les données saisies par l'utilisateur au serveur
				write(s,&categorie,1);
				//On met le dernier caratere \0 dans la chaine pour qu'il soit bien interprété
				plaqueImmatriculation[7] = '\0';
				write(s,plaqueImmatriculation,8);
				printf("En attente de réponse ...\n");
				read(s,&reponse,1);
				//Si le serveur repond par "O", c'est qu'il peut accueillir la voiture
				if (reponse == 'O') {
					//On lis le nom du serveur ( son addresse )
					read(s,&serveurReponse.sin_addr,sizeof(serveurReponse.sin_addr));
					// On convertit son nom ( de integerer à String pour qu'il soit comprehensible par l'utilisateur )
					ipServeurReponse = inet_ntoa(serveurReponse.sin_addr);
					//On fais la suite du protocole
					read(s,&dureeMaxForfait, sizeof(float));
					read(s,&prixForfait,sizeof(float));
					read(s,&prixHorsForfait,sizeof(float));
					printf("Le serveur %s a de la place pour la categorie %c.\n\tDuree max forfait : %f\n\tPrix forfait : %f\n\tPrix hors forfait : %f\n",ipServeurReponse,categorie,dureeMaxForfait,prixForfait,prixHorsForfait);
					printf("Le prix sera de : %f\n", price(dureeMaxForfait, duree, prixForfait, prixHorsForfait));
				} else if (reponse == 'N') { // Si il repond par "N", il n'y a plus de place	
					printf("Ce serveur ne peut pas accueillir la voiture");
				} else { // Sinon il repond par "E", cela veut dire que la catégorie n'existe pas
					
				}
			} else {
				printf("plaque d'immatriculation : ");
				scanf("%s", plaqueImmatriculation);	
				//On met le dernier caratere \0 dans la chaine pour qu'il soit bien interprété
				plaqueImmatriculation[7] = '\0';
				write(s,plaqueImmatriculation,8);
				char detientVoiture;
				read(s,&detientVoiture,sizeof(char));
				if (detientVoiture == 'O') {
					//On lis le nom du serveur ( son addresse )
					read(s,&serveurReponse.sin_addr,sizeof(serveurReponse.sin_addr));
					// On convertit son nom ( de integerer à String pour qu'il soit comprehensible par l'utilisateur )
					ipServeurReponse = inet_ntoa(serveurReponse.sin_addr);
					//On fait la suite du protocole
					float dureeVoitureHeures;
					read(s,&dureeVoitureHeures,sizeof(float));
					read(s,&dureeMaxForfait, sizeof(float));
					read(s,&prixForfait,sizeof(float));
					read(s,&prixHorsForfait,sizeof(float));

					

					printf("[%s] Duree : %f, Duree max forfait : %f, Prix forfait : %f, Prix Hors Forfait : %f\n",ipServeurReponse,dureeVoitureHeures,dureeMaxForfait,prixForfait,prixHorsForfait);
					printf("vous devez payer : %f\n", price(dureeMaxForfait, duree, prixForfait, prixHorsForfait));
				}
			}
		}
	}
	close(s);
}
