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
	serveur.sin_port = htons(7979);	
	serveur.sin_addr.s_addr = inet_addr("192.168.0.102");
	int s = socket(AF_INET,SOCK_STREAM,0);
	char plaqueImmatriculation[] = "AB333EA\0";
	char categorie[] = "A\0";
	int duree = 20;
	if (s < 0) {
		perror("Erreur de socket");
	} else {
		printf("Connecté au serveur\n");
		if (connect(s,(struct sockaddr*) &serveur,sizeof(serveur)) < 0) {
			perror("Erreur de connexion");
		}else {
			write(s,plaqueImmatriculation,8);
			write(s,categorie,2);
			write(s,&duree,sizeof(int));
			printf("Message envoyé");
		}
	}
}