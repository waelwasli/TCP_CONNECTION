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
#include "mabib.h"

int main ()
{ 
	int socketfd;
	struct sockaddr_in serv_addr;
	char buffer[256];
	socketfd=socket(AF_INET, SOCK_STREAM,0);
	if (socketfd <0) 
	{ 
		printf ("erreur de creation");
		exit(-1);
	}
	serv_addr.sin_addr.s_addr=inet_addr("192.168.1.110");
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_port=htons(30000);
	if(connect(socketfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0) 
	{
		printf ("erreur de connection\n"); exit(-1);
	}
	printf("connecte\n");
	int choix1,choix2,ref,nb,test,ref_ag;
	printf("choisisser un numero svp \nVous etes :\n");
	printf("1-Compagnie aerienne\n2-Agence\n");
	scanf("%d",&choix1);
	if (send(socketfd, &choix1, sizeof(choix1), 0) == -1) {
        	perror("Erreur lors de l'envoi des données au serveur 1");
        	exit(EXIT_FAILURE);
    	}
	if (choix1==1){
		printf("choisissez un numero svp \nVous voulez :\n");
		printf("1-Consulter une vol\n");
		printf("2-Consulter la facture d’une agence\n");
		printf("3-Consulter l’historique des transactions\n");
		printf("4-Ajouter une vol\n");
		scanf("%d",&choix2);
		if (send(socketfd, &choix2, sizeof(choix2), 0) == -1) {
        		perror("Erreur lors de l'envoi des données au serveur 2");
        		exit(EXIT_FAILURE);
    		}
		if(choix2==1){
			printf("Donner la reference de vol : ");
			scanf("%d",&ref);
			if (send(socketfd, &ref, sizeof(ref), 0) == -1) {
        			perror("Erreur lors de l'envoi des données au serveur 3");
        			exit(EXIT_FAILURE);
    			}
			VOL vol;
			bzero((char *) &vol, sizeof(vol));
    			int n = read(socketfd, &vol, sizeof(vol));
			if (n < 0) {
    				perror("Erreur de lecture du socket");
    				exit(1);
    			}
    			if(vol.ref_vol != -1){
    				afficher_vol(vol);
    			}
    			else printf("Erreur : vol avec référence %d non trouvé dans le fichier vol.txt\n", vol.ref_vol);	
		}
		else if(choix2==2){
			printf("Donner la reference de l'agence : ");
			scanf("%d",&ref);
			if (send(socketfd, &ref, sizeof(ref), 0) == -1) {
        			perror("Erreur lors de l'envoi des données au serveur 3");
        			exit(EXIT_FAILURE);
    			}
			FACTURE fac;
			bzero((char *) &fac, sizeof(fac));
    			int n = read(socketfd, &fac, sizeof(fac));
			if (n < 0) {
    				perror("Erreur de lecture du socket");
    				exit(1);
    			}
    			if(fac.ref_agence != -1){
    				afficher_fac(fac);
    			}
    			else printf("Erreur : agence avec référence %d non trouvé dans le fichier facture.txt\n", fac.ref_agence);	
		}
		else if(choix2==3){
			RESERVATION res;
    			ssize_t received;
    			while ((received = recv(socketfd, &res, sizeof(RESERVATION), 0)) > 0) {
    			    afficher_res(res);
    			    printf("-------------------------------------------\n");
    			}

    			if (received == -1) {
    			    perror("recv");
    			    exit(EXIT_FAILURE);
    			}
    		}
    		else if(choix2==4){
    			VOL v;
    			saisir_vol(&v);
    			if (send(socketfd, &v, sizeof(VOL), 0) == -1) {
        		perror("Erreur lors de l'envoi des données au serveur");
        		exit(EXIT_FAILURE);
    			}
    		}
	}
	else if (choix1==2){
		printf("Donner la reference de l'agence : ");
		scanf("%d",&ref);
		if (send(socketfd, &ref, sizeof(ref), 0) == -1) {
        		perror("Erreur lors de l'envoi des données au serveur 3");
        		exit(EXIT_FAILURE);
    		}
		printf("choisissez un numero svp \nVous voulez :\n");
		printf("1-Demande de reservation\n");
		printf("2-Annulation de reservation\n");
		printf("3-Recevoir une facture\n");
		scanf("%d",&choix2);
		if (send(socketfd, &choix2, sizeof(choix2), 0) == -1) {
        		perror("Erreur lors de l'envoi des données au serveur 1");
        		exit(EXIT_FAILURE);
    		}
    		if(choix2==1){
			printf("Donner la reference de vol à reserver : ");
			scanf("%d",&ref);
			if (send(socketfd, &ref, sizeof(ref), 0) == -1) {
        			perror("Erreur lors de l'envoi des données au serveur 3");
        			exit(EXIT_FAILURE);
    			}
    			printf("Donner le nombre de place : ");
			scanf("%d",&nb);
			if (send(socketfd, &nb, sizeof(int), 0) == -1) {
        			perror("Erreur lors de l'envoi des données au serveur 3");
        			exit(EXIT_FAILURE);
    			}
    			if (recv(socketfd, &test, sizeof(int),0)<0) {
    			    perror("Erreur de lecture du socket");
    			    exit(1);
    			}
    			if(test==1){
   				printf("Reservation terminee avec succes !!!\n");
   			}
   			else {
   				printf("Nombre de place disponible est insufisant !!!\n");
   			}
    		
		}
		else if(choix2==2){
			printf("Donner la reference de vol à annuler: ");
			scanf("%d",&ref);
			if (send(socketfd, &ref, sizeof(ref), 0) == -1) {
        			perror("Erreur lors de l'envoi des données au serveur 3");
        			exit(EXIT_FAILURE);
    			}
    			printf("Donner le nombre de place : ");
			scanf("%d",&nb);
			if (send(socketfd, &nb, sizeof(int), 0) == -1) {
        			perror("Erreur lors de l'envoi des données au serveur 3");
        			exit(EXIT_FAILURE);
    			}
    			if (recv(socketfd, &test, sizeof(int),0)<0) {
    			    perror("Erreur de lecture du socket");
    			    exit(1);
    			}
    			if(test==1){
   				printf("Annulation terminee avec succes !!!\n");
   			}
   			else {
   				printf("Reference du vol est incorrecte !!!\n");
   			}
    		
		}
		else if(choix2==3){
			FACTURE fac;
			bzero((char *) &fac, sizeof(fac));
    			int n = read(socketfd, &fac, sizeof(fac));
			if (n < 0) {
    				perror("Erreur de lecture du socket");
    				exit(1);
    			}
    			if(fac.ref_agence != -1){
    				afficher_fac(fac);
    			}
    			else printf("Erreur : agence avec référence %d non trouvé dans le fichier facture.txt\n", fac.ref_agence);	
		}
	}
	
	close(socketfd);
	return 0; 
}
