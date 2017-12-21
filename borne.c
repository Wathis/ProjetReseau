#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main (int argc, char *argv[]) {
	int connectionInfo; 
	struct sockaddr_in serveur;
	serveur.sin_family = AF_INET;
	serveur.sin_port = htons(7777);	
	serveur.sin_addr.s_addr = inet_addr(argv[1]);
	int s = socket(AF_INET,SOCK_STREAM,0);
	char plaqueImmatriculation[7];
	char categorie = 'A';
	int duree;


	printf("catégorie : ");
	scanf("%c", &categorie);
	printf("plaque d'immatriculation : ");
	scanf("%s", plaqueImmatriculation);	
	printf("duree : ");
	scanf("%d", &duree);




	printf("%s \n", plaqueImmatriculation);
	printf("%c \n", categorie);
	printf("%d \n", duree);


	if (s < 0) {
		perror("Erreur de socket");
	} else {
		if (connect(s,(struct sockaddr*) &serveur,sizeof(serveur)) < 0) {
			perror("Erreur de connexion");
		}else {
			printf("Connecté au serveur\n");
			/*write(s,plaqueImmatriculation,8);
			write(s,categorie,1);
			write(s,&duree,sizeof(int));
			printf("Message envoyé");*/
		}
	}
	close(s);
}
