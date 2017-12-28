#include "borne.h"

void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
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

	do {
		printf("1 pour tarif\n2 pour payer\n");
		scanf("%c", &repon);
	} while (repon != '1' &&  repon != '2');
	
	if(repon == '1') {
		viderBuffer();

	//On saisi les informations du client 
	printf("catégorie : ");
	scanf("%c", &categorie);
	printf("plaque d'immatriculation : ");
	scanf("%s", plaqueImmatriculation);	
	printf("duree : ");
	scanf("%d", &duree);

	}
	else {

	}
	
	
	
	

	// strcpy(plaqueImmatriculation,"AAABBCC\0");
	// categorie = 'B';
	// duree = 2;
	

	printf("%s \n", plaqueImmatriculation);
	printf("%c \n", categorie);
	printf("%d \n", duree);
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
	
			if(repon == '1') {
				viderBuffer();

				//On saisi les informations du client 
				printf("catégorie : ");
				scanf("%c", &categorie);
				printf("plaque d'immatriculation : ");
				scanf("%s", plaqueImmatriculation);	
				printf("duree : ");
				scanf("%d", &duree);


				//On envoie au serveur si on veut payer ou voir les tarifs
				//write(s, &repon);
				//On envoie les données saisies par l'utilisateur au serveur
				write(s,&categorie,1);
				//On met le dernier caratere \0 dans la chaine pour qu'il soit bien interprété
				plaqueImmatriculation[7] = '\0';
				write(s,plaqueImmatriculation,8);
				write(s,&duree,sizeof(int));
				printf("En attente de réponse ...\n");
				char reponse;
				struct sockaddr_in serveurReponse;
				char *ipServeurReponse;
				float prixForfait;
				float prixHorsForfait;
				read(s,&reponse,1);
				//Si le serveur repond par "O", c'est qu'il peut accueillir la voiture
				if (reponse == 'O') {
					//On lis le nom du serveur ( son addresse )
					read(s,&serveurReponse.sin_addr,sizeof(serveurReponse.sin_addr));
					// On convertit son nom ( de integerer à String pour qu'il soit comprehensible par l'utilisateur )
					ipServeurReponse = inet_ntoa(serveurReponse.sin_addr);
					//On fais la suite du protocole
					read(s,&prixForfait,sizeof(float));
					read(s,&prixHorsForfait,sizeof(float));
					printf("Le serveur %s a de la place pour la categorie %c.\n\tPrix forfait : %f\n\tPrix hors forfait : %f\n",ipServeurReponse,categorie,prixForfait,prixHorsForfait);
				} else { // Sinon il repond par "N" 	
					printf("Ce serveur ne peut pas accueillir la voiture");
				}

			}
			else {

			}



			
		}
	}
	close(s);
}
