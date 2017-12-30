# ProjetReseau

# Protocole pour la communication entre un parking et une borne

Pour ce projet, nous avons choisi d’opter pour le protocole de couche 4 nommé TCP. 

Une fois la connexion établie entre la borne et le(s) serveur(s), il faut choisir entre deux types de communications : 
	
1.	Proposition de contrat de stationnement 
2.	Etat du coût de stationnement pour une voiture donnée

Pour choisir entre les deux, il faut envoyer au(x) serveur(s) : 
	
-	(Integer 4 octets) : Numéro du type de communication (1 ou 2 comme au-dessus)

La suite du protocole est donc maintenant différente pour les deux modes de communications.

# Protocole 1 : Mode de proposition de contrat de stationnement 

Pour ce mode, il faut que la borne envoie ces informations au(x) serveur(s) : 
	
-	(Char 1 octet) Catégorie du véhicule (Lettre majuscule) 
-	(8 char de 1 octet) Plaque immatriculation du véhicule (8 caractères car une plaque d’immatriculation fait 7 caractères et il ne faut pas oublier le \0)
 
Le serveur répond ensuite par cela : 
	
-	(Char 1 octet) Type de réponse du serveur. Cette réponse peut prendre plusieurs valeurs : 
o	O : Le serveur peut accueillir la voiture de catégorie X
o	N : Le serveur ne peut pas accueillir la voiture de catégorie X
o	E : La catégorie envoyée n’existe pas 

Si le serveur a répondu autre chose que O au Type de réponse, alors il n’enverra pas plus de données. Cependant, si le type de réponse est O, alors le serveur enverra ceci : 

-	(Integer 4 octets) Nom du serveur (Son adresse IP au format Integer : Voir explication à la fin)
-	(Float 4 octets) Durée maximum pour le forfait 
-	(Float 4 octets) Prix du forfait par heure
-	(Float 4 octets) Prix hors forfait par heure (Une fois durée max dépassée)



# Protocole 2 : Mode état du coût de stationnement pour une voiture donnée 

Pour ce mode, il faut que la borne envoie ces informations au(x) serveur(s) : 
	
-	 (8 char de 1 octet) Plaque immatriculation du véhicule (8 caractères car une plaque d’immatriculation fait 7 caractères et il ne faut pas oublier le \0) 

Le serveur répond ensuite par cela : 
	
-	(Char 1 octet) Détient la voiture. Deux types de réponses possibles :
o	O : Le serveur détient la voiture dans son parking
o	N : Le serveur ne détient pas la voiture dans son parking 

Si le serveur a répondu autre chose que O, alors il n’enverra pas plus de données. Cependant, si celui-ci a bien répondu O, alors le serveur enverra ceci : 

-	(Integer 4 octets) Nom du serveur (Son adresse IP au format long Integer) 
-	(Float 4 octets) Durée de stationnement actuelle de la voiture 
-	(Float 4 octets) Durée maximum pour le forfait 
-	(Float 4 octets) Prix du forfait par heure
-	(Float 4 octets) Prix hors forfait par heure (Une fois durée max dépassée)

# Explication du nom du serveur au format integer

Nous avons choisi de faire transiter l’adresse du serveur sur le réseau sous forme d’integer. En effet, une adresse IPv4 détient 4 octets. Ces octets peuvent être stockés dans un integer. Celui-ci pourra être par la suite analysé côté serveur / client pour en extraire, octet par octet, chaque nombre de l’IP. Ce principe permet d’éviter de faire transiter des caractères sur le réseau qui prendraient plus d’octets et d’avoir une taille connue pour lire l’adresse.

Voici un exemple pour extraire le premier nombre d’une adresse IPv4 au format integer :

Le client reçoit en décimal :      3232235558
Ce qui donne en binaire :          11000000 10101000 00000000 00100110 (3232235558)

Nous lui appliquons (ET) avec :    11111111 00000000 00000000 00000000 (4278190080)

Ce qui donne :  3232235558 & 4278190080	 = 3221225472
                                   11000000 00000000 00000000 00000000 (3221225472)

Nous voulons le premier nombre de l’adresse IPv4 (Donc le premier octet), nous divisons donc par le premier bit du premier octet que l’on veut extraire, représenté en rouge au-dessus.
				                           00000001 00000000 00000000 00000000 (16777216)
Ce qui donne : 3221225472 / 16777216 = 192. 

En répétant ce principe sur les autres octets, on obtient l’adresse IPv4 : 192.168.0.38.
