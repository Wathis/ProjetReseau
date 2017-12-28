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

	//index 0 correspond au forfait A, 1 correspond forfait B
	float prixForfaits[] = {2,3};
	float prixHorsForfaits[] = {5,8};
	int nbPlacesParCategories[] = {2,2};

	int NBR_FORFAITS = sizeof(prixForfaits) / sizeof(prixForfaits[0]);

	char plaqueImmatriculation[8];
	char categorie;
	char menu;
	int duree ;
	int socketClient;
	float prixForfait;
	float prixHorsForfait;

	//while (1) {
		socketClient = accept(socketFd, (struct sockaddr *)&client, &add_client);
		printf("Tentative de connexion ...\n");
		if (socketClient > 0 ) {
			if (fork() == 0) {
				printf("-------------------------------------\n");
				printf("/!\\ Demande connexion client accepté\n");
				printf("-------------------------------------\n");
				read(socketClient,&menu, 1);
				printf("%c\n", menu);
				if (menu == '1')
				{
					read(socketClient,&categorie,1);
					read(socketClient,plaqueImmatriculation,8);
					read(socketClient,&duree,sizeof(int));
					printf("/INFO\\ Demande de la plaque : %s categorie : %c pour duree : %d\n\n",plaqueImmatriculation,categorie, duree);
					if (categorie - 'A' < NBR_FORFAITS) {
					prixForfait = prixForfaits[categorie - 'A'];
					prixHorsForfait = prixHorsForfaits[categorie - 'A'];
					printf("Envoie de %f  %f \n",prixForfait,prixHorsForfait);
					//On envoie la reponse au client, avec le prix forfait et prix hors forfait correspondant a sa catégorie de vehicule
					if (nbPlacesParCategories[categorie - 'A'] > 0) {
						// "O" permet de dire que oui on a de la place
						write(socketClient,"O",1);
						// On donne le nom du serveur au client ( son addresse au format integer ) 
						write(socketClient,&serveur.sin_addr,sizeof(serveur.sin_addr));
						//On fait la suite du protocole
						write(socketClient,&prixForfait,sizeof(float));
						write(socketClient,&prixHorsForfait,sizeof(float));
					} else {
						//Il n'y a plus de place pour la categorie demandée
						write(socketClient,"N",1);
					}
				}
				
				} else {
					//Le forfait n'existe pas 
					write(socketClient,"N",1);
				}
				close(socketClient);
				printf("Communication fermée\n");
			}
		} else {
			perror("Erreur de accept\n");	
		}
	//}
	close(socketFd);
	
}
