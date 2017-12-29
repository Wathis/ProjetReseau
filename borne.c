#include "borne.h"

int main (int argc, char *argv[]) {

	char plaqueImmatriculation[8];
	char categorie;
	int dureeSouhaiteeMinutes;
	float dureeSouhaiteeHeures;

	//On lance le menu principal
	int choixMenu = menuPrincipal();

	//Dans les deux protocoles il faut la plaque, on la demande donc avant
	printf("Plaque d'immatriculation : ");
	scanf("%s", plaqueImmatriculation);	
	//On met le dernier caratere \0 dans la chaine pour qu'il soit bien interprété
	plaqueImmatriculation[7] = '\0';
	viderBuffer();

	if (choixMenu == 1) {
		//On saisi les informations du client 
		printf("Catégorie : ");
		scanf("%c", &categorie);
		viderBuffer();
		printf("Duree en minutes que vous pensez rester : ");
		scanf("%d", &dureeSouhaiteeMinutes);
		//On convertit en heures
		dureeSouhaiteeHeures = (float) dureeSouhaiteeMinutes / (float) 60;
		viderBuffer();
	}

	//On boucle sur tous les serveurs existants pour tous les interroger
	for (int i = 0 ; i < NB_SERVEURS; i++) {
		//On créé un nouveau processus pour chaque serveur pour les interroger simultanement
		if (fork() == 0) {
			// On stock l'addresse du serveur a tester configuré dans CONFIG_SERVEUR.h
			struct sockaddr_in serveur;
			serveur.sin_family = AF_INET;
			serveur.sin_port = htons(atoi(SERVEURS[i + 1]));
			serveur.sin_addr.s_addr = inet_addr(SERVEURS[i]);

			//On créé la socket du serveur
			int socketServeur = socket(AF_INET,SOCK_STREAM,0);
			// Test de la validité de la socket
			if (socketServeur < 0) {
				perror("Erreur de socket");
			} else {
				//On se connecte la socket du serveur a l'addresse correspondante
				if (connect(socketServeur,(struct sockaddr*) &serveur,sizeof(serveur)) < 0) {
					perror("Erreur de connexion");
				} else {
					//On envoie au serveur si on veut payer ou voir les tarifs ( Choix du protocole )
					write(socketServeur, &choixMenu,sizeof(int));
					//Si choixMenu == 1 : Proposition de contrat de stationnement pour une voiture donnée
					if(choixMenu == 1) {
						premierProtocole(socketServeur,SERVEURS[i],plaqueImmatriculation,dureeSouhaiteeHeures,categorie);
					} else { //Sinon un état du coût de stationnement pour une voiture donnée
						deuxiemeProtocole(socketServeur,SERVEURS[i],plaqueImmatriculation);
					}
				}
			}
			close(socketServeur);
			exit(0);
		}
	}
}

void premierProtocole(int socketServeur,char ipServeur[],char plaqueImmatriculation[],float dureeSouhaiteeHeures,char categorie) {
	//Variables utiles
	char reponse;
	struct sockaddr_in serveurReponse;
	char *ipServeurReponse;
	float dureeMaxForfait;
	float prixForfait;
	float prixHorsForfait;

	//On envoie les données saisies par l'utilisateur au serveur
	write(socketServeur,&categorie,1);
	write(socketServeur,plaqueImmatriculation,8);
	read(socketServeur,&reponse,1);
	//Si le serveur repond par "O", c'est qu'il peut accueillir la voiture
	if (reponse == 'O') {
		//On lis le nom du serveur ( son addresse )
		read(socketServeur,&serveurReponse.sin_addr,sizeof(serveurReponse.sin_addr));
		// On convertit son nom ( de integerer à String pour qu'il soit comprehensible par l'utilisateur )
		ipServeurReponse = inet_ntoa(serveurReponse.sin_addr);
		//On fais la suite du protocole
		read(socketServeur,&dureeMaxForfait, sizeof(float));
		read(socketServeur,&prixForfait,sizeof(float));
		read(socketServeur,&prixHorsForfait,sizeof(float));
		printLines(45);
		printf("[%s] a de la place pour la categorie %c.\n\tDuree max forfait : %f heure(s)\n\tPrix forfait : %f € / H\n\tPrix hors forfait : %f € / H\n",ipServeurReponse,categorie,dureeMaxForfait,prixForfait,prixHorsForfait);
		printf("\tLe prix sera de : \n\t\t%f € pour votre durée souhaitée \n", price(dureeMaxForfait, dureeSouhaiteeHeures, prixForfait, prixHorsForfait));
		printf("\t\t%f € pour une heure de retard\n", price(dureeMaxForfait, dureeSouhaiteeHeures + 1, prixForfait, prixHorsForfait));
		printf("\t\t%f € pour une durée double\n", price(dureeMaxForfait, dureeSouhaiteeHeures * (float) 2, prixForfait, prixHorsForfait));
	} else if (reponse == 'N') { // Si il repond par "N", il n'y a plus de place	
		printf("Ce serveur ne peut pas accueillir la voiture\n");
	} else if (reponse == 'E'){ // Sinon il repond par "E", cela veut dire que la catégorie n'existe pas
		printf("La catégoire précisée n'existe pas\n");
	} else {
		printf("Une erreur est survenue\n");
	}
}

void deuxiemeProtocole(int socketServeur,char ipServeur[],char plaqueImmatriculation[]) {
	//Variables utiles
	char reponse;
	struct sockaddr_in serveurReponse;
	char *ipServeurReponse;
	float dureeMaxForfait;
	float prixForfait;
	float prixHorsForfait;

	write(socketServeur,plaqueImmatriculation,8);
	char detientVoiture;
	read(socketServeur,&detientVoiture,sizeof(char));
	if (detientVoiture == 'O') {
		//On lis le nom du serveur ( son addresse )
		read(socketServeur,&serveurReponse.sin_addr,sizeof(serveurReponse.sin_addr));
		// On convertit son nom ( de integerer à String pour qu'il soit comprehensible par l'utilisateur )
		ipServeurReponse = inet_ntoa(serveurReponse.sin_addr);
		//On fait la suite du protocole
		float dureeVoitureHeures;
		read(socketServeur,&dureeVoitureHeures,sizeof(float));
		read(socketServeur,&dureeMaxForfait, sizeof(float));
		read(socketServeur,&prixForfait,sizeof(float));
		read(socketServeur,&prixHorsForfait,sizeof(float));

		printf("[%s] Detient la voiture : %s\n\tDuree en stationement : %f heure(s)\n\tDuree max forfait : %f heure(s)\n\tPrix forfait : %f € / H\n\tPrix Hors Forfait : %f € / H\n",ipServeurReponse,plaqueImmatriculation,dureeVoitureHeures,dureeMaxForfait,prixForfait,prixHorsForfait);
		printf("\t[INFO] Vous devez payer : %f €\n", price(dureeMaxForfait, dureeVoitureHeures, prixForfait, prixHorsForfait));
	} else {
		printf("[%s] Ne detient pas la voiture identifiée par : %s\n", ipServeur,plaqueImmatriculation);
	}
}

int menuPrincipal() {
	char repon = '0';
	afficherMessage("Bienvenue dans une borne de parking");
	printf("Choisir une action : \n");
	printf("\t- 1 pour tarif\n\t- 2 pour payer\n");
	do {
		printf("> ");
		scanf("%c", &repon);
		viderBuffer();
		if (repon != '1' &&  repon != '2') {
			printf("Le menu %c n'existe pas !\n",repon);
		}
	} while (repon != '1' &&  repon != '2');

	//On retourne la reponse au format integer
	return repon - '0';
}

void afficherMessage(char message[]) {
	printLines(50);
	printf("\t%s\n",message);
	printLines(50);
}

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

void printLines(int number) {
	for (int i = 0 ; i < number ; i++) {
		printf("-");
	}
	printf("\n");
}
