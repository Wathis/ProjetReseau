#include "serveur.h"


int main (int argc, char *argv[]) {

	int socketServeur = socket(AF_INET, SOCK_STREAM,0);
	
	struct sockaddr_in serveur;

	//On recupere l'addresse du serveur donnée en parametres
	if (argc > 2) { 
		//Configuration de l'addresse du serveur
    	inet_aton(argv[1], &serveur.sin_addr);
		serveur.sin_family = AF_INET;
		serveur.sin_port = htons(atoi(argv[2]));
	} else {
		perror("Vous devez entrer l'addresse du serveur et le port comme ceci : \n\t./serveur 192.168.0.1 7777");
		exit(1);
	}
	//On regarde si il n'y a pas d'erreur dans le bind
	if (bind(socketServeur,(struct sockaddr *) &serveur, sizeof(serveur)) < 0) {
		perror("Erreur dans le bind");
		exit(1);
	} else {
		printf("Binding ... \n");
	}
	//On fais écouter notre serveur sur un port
	if (listen(socketServeur,1) < 0 ) {
		perror("Erreur dans le listen");
		exit(1);
	} else {
		printf("[%s] Ecoute sur %d... \n",argv[1],atoi(argv[2]));
    }

    //Allocation d'un espace mémoire pour une addresse client
	struct sockaddr_in client;
	client.sin_family = AF_INET;
	unsigned int add_client = sizeof(client);

	mettreVoitureDansParking();
	
	int socketClient;

	while (1) {
		socketClient = accept(socketServeur, (struct sockaddr *)&client, &add_client);
		printf("Tentative de connexion ...\n");
		if (socketClient > 0 ) {
			if (fork() == 0) {
				choisirUnProtocole(socketClient,serveur);
			}
		} else {
			perror("Erreur de accept\n");	
		}
	}
	close(socketServeur);
}

void mettreVoitureDansParking() {
	//On stocke une voiture de categorie A par default pour les tests
	strcpy(voituresPresentes[0].plaque,"AAABBCC\0");
	voituresPresentes[0].timeStampArrivee = time(0) - 1000; // Timestamp actuel
	voituresPresentes[0].categorie = 'A';
}

void choisirUnProtocole(int socketClient,struct sockaddr_in serveur) {
	int menu;

	printf("-------------------------------------\n");
	printf("/!\\ Demande connexion client accepté\n");
	printf("-------------------------------------\n");

	//On lis le choix que la borne a fais dans le menu
	read(socketClient,&menu,sizeof(int));

	switch (menu) {
		case 1 : //Si menu == 1 c'est que la borne veut une proposition de contrat de stationnement pour une voiture donnée
			premierProtocole(socketClient,serveur);
			break;
		case 2 : //Si menu == 2 c'est qu'elle veut connaitre un état du coût de stationnement pour une voiture donnée
			deuxiemeProtocole(socketClient,serveur);
			break;
		default : 
			printf("Ce protocole n'existe pas\n");
			break;
	}

	close(socketClient);
	printf("Communication fermée avec le client\n");
}

void premierProtocole(int socketClient,struct sockaddr_in serveur) {
	//Variables utiles 
	char plaqueImmatriculation[8];
	char categorie;
	float dureeMaxForfait;
	float prixForfait;
	float prixHorsForfait;
	//On commence à suivre le protocole 
	read(socketClient,&categorie,1);
	read(socketClient,plaqueImmatriculation,8);
	printf("/INFO\\ Demande de la plaque : %s categorie : %c \n\n",plaqueImmatriculation,categorie);
	if (categorie - 'A' < NBR_FORFAITS) {
		//On extrait les valeurs correspondantes au type de categorie 
		dureeMaxForfait = dureeMaxForfaits[categorie - 'A'];
		prixForfait = prixForfaits[categorie - 'A'];
		prixHorsForfait = prixHorsForfaits[categorie - 'A'];
		//On envoie la reponse au client, avec le prix forfait et prix hors forfait correspondant a sa catégorie de vehicule
		if (nbPlacesLibresParCategories[categorie - 'A'] > 0) {
			// "O" permet de dire que oui on a de la place
			write(socketClient,"O",1);
			// On donne le nom du serveur au client ( son addresse au format integer ) 
			write(socketClient,&serveur.sin_addr,sizeof(serveur.sin_addr));
			//On fait la suite du protocole en envoyant les données correspondantes dans l'ordre
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
}

void deuxiemeProtocole(int socketClient,struct sockaddr_in serveur) {
	//Variables utiles 
	char plaqueImmatriculation[8];
	char categorie;
	float dureeMaxForfait;
	float prixForfait;
	float prixHorsForfait;

	//On commence à suivre le protocole 
	read(socketClient,plaqueImmatriculation,8);

	int indexVoiture = rechercheVoitureParking(plaqueImmatriculation);
	
	//Si -1 C'est que l'on a pas trouvé la voiture
	if (indexVoiture == -1) {
		//On renvoie 'N' au client pour dire que on ne l'a pas trouvé
		write(socketClient,"N",sizeof(char));
	} else { //Sinon c'est qu'elle est bien dans le parking, on peut alors renvoyer les informations au client
		//On renvoie 'O' au client pour dire que l'on a sa voiture
		write(socketClient,"O",sizeof(char));

		//On va chercher les differentes informations en fonctions de la categorie du vehicule
		categorie = voituresPresentes[indexVoiture].categorie;
		dureeMaxForfait = dureeMaxForfaits[categorie - 'A'];
		prixForfait = prixForfaits[categorie - 'A'];
		prixHorsForfait = prixHorsForfaits[categorie - 'A'];

		// On donne le nom du serveur au client ( son addresse ) 
		write(socketClient,&serveur.sin_addr,sizeof(serveur.sin_addr));

		//On extrait la durée a laquelle est resté la voiture dans le parking
		long int dureeVoiture = time(0) - voituresPresentes[indexVoiture].timeStampArrivee;
		float dureeVoitureHeures = (float) dureeVoiture / (float) 3600; //On repasse le timestamp secondes en heures
		//On fait la suite du protocole en envoyant les données correspondantes dans l'ordre
		write(socketClient,&dureeVoitureHeures,sizeof(float));
		write(socketClient,&dureeMaxForfait,sizeof(float));
		write(socketClient,&prixForfait,sizeof(float));
		write(socketClient,&prixHorsForfait,sizeof(float));
	}
}

int rechercheVoitureParking(char plaqueImmatriculation[]) {
	//On parcours les voitures presentes pour voir si celle demandée est presente
	int i = 0;
	for (i = 0 ; i < NBR_MAX_PLACE ; i++) {
		if (strcmp(voituresPresentes[i].plaque,plaqueImmatriculation) == 0) {
			return i;
		}
	}
	//Return -1 veut dire que on n'a pas trouvé la voiture
	return -1;
}
