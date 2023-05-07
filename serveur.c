#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h> 
#include <unistd.h>
#include <netdb.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include "mabib.h"


sem_t sem;

void *connection_handler(void *sc){
	
	int newsocket = *(int *)sc;
	int choix1,choix2,ref,nb,ref_ag;
	sem_wait(&sem); // Entrée de la section critique
    	if (recv(newsocket, &choix1, sizeof(choix1),0)<0) {
    	    perror("Erreur de lecture du socket");
    	    exit(1);
    	}
    	printf("choix 1 = %d\n",choix1); 
    	if (choix1==1){
    		if (recv(newsocket, &choix2, sizeof(choix2),0)<0) {
    		    perror("Erreur de lecture du socket");
    		    exit(1);
    		}
    		printf("choix 2 = %d\n",choix2);
    		if(choix2==1){
    			if (recv(newsocket, &ref, sizeof(ref),0)<0) {
    			perror("Erreur de lecture du socket");
    	    		exit(1);
    			}
    			printf("Ref = %d\n",ref);
    			VOL v=afficher_vol_fichier(ref);
    			if (send(newsocket, &v, sizeof(VOL), 0) == -1) {
        		perror("Erreur lors de l'envoi des données au serveur");
        		exit(EXIT_FAILURE);
    			}
    		}
    		else if (choix2==2){
    			if (recv(newsocket, &ref, sizeof(ref),0)<0) {
    			perror("Erreur de lecture du socket");
    	    		exit(1);
    			}
    			printf("Ref = %d\n",ref);
    			FACTURE f=afficher_fac_fichier(ref);
    			if (send(newsocket, &f, sizeof(FACTURE), 0) == -1) {
        		perror("Erreur lors de l'envoi des données au serveur");
        		exit(EXIT_FAILURE);
    			}
    		}
    		else if (choix2==3){
    			FILE *fichier = fopen("histo.txt", "r");
    			if (fichier == NULL) {
        		printf("Impossible d'ouvrir le fichier histo.txt");
    			}
    			RESERVATION res;
    			// Lire chaque réservation du fichier histo.txt et l'envoyer via la connexion TCP
    			while (fscanf(fichier, "Référence: %d\tAgence : %d\tType : %s\tValeur : %d\tRésultat : %s\n", &res.ref_vol, &res.agence,res.transaction,&res.valeur, res.resultat)== 5) {
        			send(newsocket, &res, sizeof(RESERVATION), 0);
    			}
    		}
    		else if (choix2==4){
    			VOL vol;
			bzero((char *) &vol, sizeof(vol));
    			int n = read(newsocket, &vol, sizeof(vol));
			if (n < 0) {
    				perror("Erreur de lecture du socket");
    				exit(1);
    			}
    			ecrire_vol_fichier(vol);
    			afficher_vol(vol);	
		}
    	}
    	else if(choix1==2){
    		if (recv(newsocket, &ref_ag, sizeof(int),0)<0) {
    	    		perror("Erreur de lecture du socket");
    	    		exit(1);
    		}
    		printf("Reference de l'agence = %d\n",choix1);
    		if (recv(newsocket, &choix2, sizeof(choix2),0)<0) {
    		    perror("Erreur de lecture du socket");
    		    exit(1);
    		}
    		if(choix2==1){
    			if (recv(newsocket, &ref, sizeof(ref),0)<0) {
    				perror("Erreur de lecture du socket");
    	    			exit(1);
    			}
    			printf("Ref = %d\n",ref);
    			if (recv(newsocket, &nb, sizeof(int),0)<0) {
    	    			perror("Erreur de lecture du socket");
    	    			exit(1);
    			}	
    			printf("nb = %d\n",nb);
    			int test=reserver(ref_ag,ref,nb);
    			if (send(newsocket, &test, sizeof(int), 0) == -1) {
        			perror("Erreur lors de l'envoi des données au serveur 2");
        			exit(EXIT_FAILURE);
    			}
    		}
    		else if(choix2==2){
    			if (recv(newsocket, &ref, sizeof(ref),0)<0) {
    				perror("Erreur de lecture du socket");
    	    			exit(1);
    			}
    			printf("Ref = %d\n",ref);
    			if (recv(newsocket, &nb, sizeof(int),0)<0) {
    	    			perror("Erreur de lecture du socket");
    	    			exit(1);
    			}	
    			printf("nb = %d\n",nb);
    			int test=annuler(ref_ag,ref,nb);
    			if (send(newsocket, &test, sizeof(int), 0) == -1) {
        			perror("Erreur lors de l'envoi des données au serveur 2");
        			exit(EXIT_FAILURE);
    			}
    		}
    		else if(choix2==3){
    			FACTURE f=afficher_fac_fichier(ref_ag);
    			if (send(newsocket, &f, sizeof(FACTURE), 0) == -1) {
        		perror("Erreur lors de l'envoi des données au serveur");
        		exit(EXIT_FAILURE);
    			}
    		}
    	}
    	sem_post(&sem); // Sortie de la section critique
    	free(sc);
   }
int main ()
{ 
	
	int sc, client_sock, c, *new_sock;
	struct sockaddr_in serv_addr, client;
	
	// Initialisation du sémaphore
    	sem_init(&sem, 0, 1);
    	
    	// Création du socket
	sc=socket(AF_INET, SOCK_STREAM,0);
	if (sc <0) 
	{ 
		printf ("erreur de creation");
		 exit(-1);
	}
	
	// Préparation de la structure sockaddr_in
	serv_addr.sin_addr.s_addr=inet_addr("192.168.1.110");
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_port=htons(30000);
	
	//Bind
	if(bind(sc,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0) 
	{
		printf ("erreur de bind\n");
		exit(-1);
	}
	printf("bind : %d\n",sc);
	
	//Listen
	listen(sc,5);
	printf("Listen \n");
	
	// Accept incoming connection
    	printf("Waiting for incoming connections...\n");
    	c = sizeof(struct sockaddr_in);
    	while ((client_sock = accept(sc, (struct sockaddr *)&client, (socklen_t *)&c))) {
    	    printf("New connection accepted\n");

    	    // Création d'un nouveau thread pour gérer la connexion
    	    pthread_t thread;
    	    new_sock = malloc(1);
    	    *new_sock = client_sock;

    	    if (pthread_create(&thread, NULL, connection_handler, (void *)new_sock) < 0) {
    	        perror("could not create thread");
            	return 1;
            }

            // JOIN afin que nous ne terminions pas avant le thread
            pthread_join(thread, NULL);
            printf("Handler assigned\n");
    	}
    	
    	if (client_sock < 0) {
        	perror("accept failed");
        	return 1;
    	}
	sem_destroy(&sem);
	close(sc);
    	return 0;
}
