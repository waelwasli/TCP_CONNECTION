#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "mabib.h"
/*********************************VOL*********************************/
void saisir_vol(VOL* v)
{
	printf("donner  reference du vol : ");
	scanf("%d",&v->ref_vol);
	printf("donner la destination : ");
	scanf("%s",v->destination);
	printf("donner le nombre de place disponible  : ");
	scanf("%d",&v->place_dispo);
	printf("donner le prix d une place : ");
	scanf("%f",&v->prix_place);
}
void afficher_vol(VOL v)
{
	printf("Reference Vol : %d \nDestination : %s\nNombre de place : %d\nPrix Place : %.2f",v.ref_vol,v.destination,v.place_dispo,v.prix_place);
}

void ecrire_vol_fichier(VOL v1)
{
	 // Ouverture du fichier en mode ajout
	 FILE* fichier = fopen("vols.txt", "a"); 
   	 if (fichier == NULL) {
       		 printf("Erreur : impossible d'ouvrir le fichier\n");
       		 exit(1);
    	}
    	// Ecriture de la structure vol modifiee dans le fichier
        fprintf(fichier, "Ref Vol: %d\tDestination: %s\tPlaces Disponibles: %d\tPrix Place: %.2f\n",
           v1.ref_vol, v1.destination, v1.place_dispo, v1.prix_place);
    	fclose(fichier);
}

int reserver(int ref_ag,int ref_vol, int nb_place) {
   	// Ouverture du fichier en mode lecture
   	FILE *fichier = fopen("vols.txt", "r");
   	// Verification que le fichier est ouvert avec succes
   	if (fichier == NULL) {
     	 printf("Erreur d'ouverture du fichier.\n");
   	}
   	float somme=0;
   	VOL v_temp;
   	RESERVATION res;
   	res.ref_vol=ref_vol;
   	res.agence= ref_ag;
   	strcpy(res.transaction,"Demande");
   	res.valeur=nb_place;
   	// Ouverture d'un fichier temporaire en mode ecriture
	FILE *fichier_temp = fopen("vols_temp.txt", "w");
   	// Verification que le fichier temporaire est ouvert avec succes
   	if (fichier_temp == NULL) {
   	   printf("Erreur d'ouverture du fichier temporaire.\n");
   	   fclose(fichier);
   	}
   	// Copie de la structure vol dans le fichier temporaire, en modifiant la place_dispo si necessaire
   	int test=0;
   	while (fscanf(fichier, "Ref Vol: %d\tDestination: %s\tPlaces Disponibles: %d\tPrix Place: %f\n",
   	              &v_temp.ref_vol, v_temp.destination, &v_temp.place_dispo, &v_temp.prix_place) == 4) {
   	   if ((v_temp.ref_vol == ref_vol)&&(v_temp.place_dispo>=nb_place)) {
   	      v_temp.place_dispo -=nb_place ;
   	      somme+=nb_place*v_temp.prix_place;
   	      modifier_fac_fichier(ref_ag,somme);
   	      strcpy(res.resultat,"succes");
   	      test=1;
   	   }
   	   fprintf(fichier_temp, "Ref Vol: %d\tDestination: %s\tPlaces Disponibles: %d\tPrix Place: %.2f\n",
   	           v_temp.ref_vol, v_temp.destination, v_temp.place_dispo, v_temp.prix_place);
   	}
   	//message de verification
   	if(test==1){
   		printf("Reservation terminee avec succes !!!\n");
   	}
   	else {
   		printf("Nombre de place disponible est insufisant !!!\n");
   		strcpy(res.resultat,"impossible");
   	}
   	// Ajouter une reservation dans histo.txt
   	ecrire_res_fichier(res);
   	// Fermeture des fichiers
   	fclose(fichier);
   	fclose(fichier_temp);
   	// Suppression du fichier original
   	remove("vols.txt");
   	// Renommage du fichier temporaire en fichier original
   	rename("vols_temp.txt", "vols.txt");
   	return test;
}

int annuler(int ref_ag,int ref_vol, int nb_place) {
   	// Ouverture du fichier en mode lecture
   	FILE *fichier = fopen("vols.txt", "r");
   	// Verification que le fichier est ouvert avec succes
   	if (fichier == NULL) {
     	 printf("Erreur d'ouverture du fichier.\n");
   	}
   	float somme=0;
   	VOL v_temp;
   	RESERVATION res;
   	res.ref_vol=ref_vol;
   	res.agence=ref_ag;
   	strcpy(res.transaction,"Annulation");
   	res.valeur=nb_place;
   	strcpy(res.resultat,"succes");
   	// Ouverture d'un fichier temporaire en mode ecriture
	   FILE *fichier_temp = fopen("vols_temp.txt", "w");
   	// Verification que le fichier temporaire est ouvert avec succes
   	if (fichier_temp == NULL) {
   	   printf("Erreur d'ouverture du fichier temporaire.\n");
   	   fclose(fichier);
   	}
   	// Copie de la structure vol dans le fichier temporaire, en modifiant la place_dispo si necessaire
   	int test=0;
   	while (fscanf(fichier, "Ref Vol: %d\tDestination: %s\tPlaces Disponibles: %d\tPrix Place: %f\n",
   	              &v_temp.ref_vol, v_temp.destination, &v_temp.place_dispo, &v_temp.prix_place) == 4) {
   	   if (v_temp.ref_vol == ref_vol) {
   	      v_temp.place_dispo +=nb_place ;
   	      somme=-0.9*(nb_place*v_temp.prix_place);
   	      modifier_fac_fichier(res.agence,somme);
   	      test=1;
   	   }
   	   fprintf(fichier_temp, "Ref Vol: %d\tDestination: %s\tPlaces Disponibles: %d\tPrix Place: %.2f\n",
   	           v_temp.ref_vol, v_temp.destination, v_temp.place_dispo, v_temp.prix_place);
   	}
   	//message de verification
   	if(test==1){
   		printf("Annulation terminee avec succes !!!\n");
   		ecrire_res_fichier(res);
   	}
   	else printf("Reference du vol est incorrecte !!!\n");
   	// Fermeture des fichiers
   	fclose(fichier);
   	fclose(fichier_temp);
   	// Suppression du fichier original
   	remove("vols.txt");
   	// Renommage du fichier temporaire en fichier original
   	rename("vols_temp.txt", "vols.txt");
   	return test;
}

VOL afficher_vol_fichier(int ref) 
{
	// Ouverture du fichier en mode lecture
    	FILE *fichier = fopen("vols.txt", "r");
    	VOL v;
    	if (fichier != NULL) {
    		while (fscanf(fichier, "Ref Vol: %d\nDestination: %s\nPlaces Disponibles: %d\nPrix Place: %f\n",
    		             &v.ref_vol, v.destination, &v.place_dispo, &v.prix_place) == 4) {
    		    if (v.ref_vol == ref) {
    		        afficher_vol(v);
    		        fclose(fichier);
    		        return v;
    		    }
    		}
    		printf("Erreur : vol avec référence %d non trouvé dans le fichier vol.txt\n", ref);
    		fclose(fichier);
    	}
    	else {
        	printf("Erreur d'ouverture du fichier.\n");
   	}
    	v.ref_vol = -1;
    	return v;
    	
}

/*********************************RESERVATION*********************************/
void saisir_res(RESERVATION* res)
{
	printf("Saisir la référence du vol : ");
    	scanf("%d", &res->ref_vol);
    	printf("Saisir la reference de l agence : ");
    	scanf("%d", &res->agence);
    	printf("Saisir le type de transaction : ");
    	scanf("%s", res->transaction);
    	printf("Saisir la valeur de la transaction : ");
    	scanf("%d", &res->valeur);
    	printf("Saisir le resultat de la transaction : ");
    	scanf("%s", res->resultat);
}

void afficher_res(RESERVATION res) 
{   
	printf("Référence du vol : %d\n", res.ref_vol);
	printf("Agence : %d\n", res.agence);
    	printf("Type de transaction : %s\n", res.transaction);
    	printf("Valeur de la transaction : %d\n", res.valeur);
    	printf("Résultat de la transaction : %s\n", res.resultat);
}
void ecrire_res_fichier(RESERVATION res)
{
	// Ouverture en mode ajout à la fin
	FILE* fichier = fopen("histo.txt", "a");
    	if (fichier == NULL) {
        	printf("Erreur : Impossible d'ouvrir le fichier histo.txt\n");
        	exit(EXIT_FAILURE);
    	}
    	// Ecriture de la structure reservation dans le fichier
    	fprintf(fichier, "Référence: %d\tAgence : %d\tType : %s\tValeur : %d\tRésultat : %s\n", res.ref_vol, res.agence, res.transaction, res.valeur, res.resultat);
    	fclose(fichier);
}
void afficher_res_fichier() 
{
    	FILE *fichier = fopen("histo.txt", "r");
    	if (fichier == NULL) {
    	    printf("Impossible d'ouvrir le fichier histo.txt.\n");
    	    return;
    	}
    	
    	RESERVATION res;
    	while (fscanf(fichier, "Référence: %d\tAgence : %d\tType : %s\tValeur : %d\tRésultat : %s\n", &res.ref_vol, &res.agence,res.transaction,&res.valeur, res.resultat)== 5) {
	        afficher_res(res);
	}
	fclose(fichier);
}

/*********************************FACTURE*********************************/
void saisir_fac(FACTURE* f) {
    	printf("Saisir la référence de l agence : ");
    	scanf("%d", &f->ref_agence);
    	printf("Saisir la somme à payer : ");
    	scanf("%f", &f->somme);
}
void afficher_fac(FACTURE f)
{
	printf("Référence de l agence : %d\n", f.ref_agence);
	printf("Somme à payer : %.2f\n", f.somme);
}
void ecrire_fac_fichier(FACTURE f)
{
	// Ouverture en mode ajout à la fin
	FILE* fichier = fopen("facture.txt", "a"); 
	if (fichier == NULL) {
        	printf("Erreur : Impossible d'ouvrir le fichier facture.txt\n");
        	exit(EXIT_FAILURE);
    	}
    	// Ecriture de la structure facture dans le fichier
    	fprintf(fichier, "Référence de l agence : %d\tSomme à payer : %.2f\n", f.ref_agence, f.somme);
    	fclose(fichier);
}
FACTURE	 afficher_fac_fichier(int ref) {
    	FILE *fichier = fopen("facture.txt", "r");
    	FACTURE f;
	if(fichier!=NULL)
    	{
    		while (fscanf(fichier, "Référence de l agence : %d\tSomme à payer : %f\n", &f.ref_agence, &f.somme) == 2) {
    	    		if (f.ref_agence == ref) {
    	   			afficher_fac(f);
    	        		fclose(fichier);
    				return f;
    	    		}		
    		}
    		printf("Erreur : Agence avec référence %d non trouvé dans le fichier vol.txt\n", ref);
    		fclose(fichier);
    	}
    	else {
        	printf("Erreur d'ouverture du fichier.\n");
   	}
    	f.ref_agence = -1;
    	return f;
}
void modifier_fac_fichier(int ref_agence, float somme){
	FILE *fichier, *temp;
    	FACTURE f;

    	// Ouverture du fichier en mode ecriture
    	fichier = fopen("facture.txt", "w+");
    	if (fichier == NULL)
    	{
    	    printf("Erreur : Impossible d'ouvrir le fichier facture.txt\n");
    	    exit(EXIT_FAILURE);
    	}
	
    	// Ouverture du fichier temporaire en mode écriture
    	temp = fopen("temp.txt", "w");
    	if (temp == NULL)
    	{
    	    printf("Erreur : Impossible d'ouvrir le fichier temp.txt\n");
    	    exit(EXIT_FAILURE);
    	}
	
    	int found = 0;
    	// Lecture de chaque ligne du fichier facture.txt
    	while (fscanf(fichier, "Référence de l agence : %d\tSomme à payer : %f\n", &f.ref_agence, &f.somme) == 2)
    	{
    	    // Si la référence de l'agence correspond à celle recherchée
    	    if (f.ref_agence == ref_agence)
    	    {
    	        // Modification du champ somme de la structure FACTURE
    	        f.somme += somme;
    	        found = 1;
    	    }
    	    // Ecriture de la ligne dans le fichier temporaire
    	    fprintf(temp, "Référence de l agence : %d\tSomme à payer : %.2f\n", f.ref_agence, f.somme);
    	}
	
    	if (!found)
    	{
    	    printf("C'est votre premiere reservation !!!\n");
    	    fprintf(temp, "Référence de l agence : %d\tSomme à payer : %.2f\n", ref_agence, somme);
    	}
    	else
    	{
    	    printf("La facture a été modifiée avec succès.\n");
    	}
	
    	// Fermeture des fichiers
    	fclose(fichier);
    	fclose(temp);
	
    	// Suppression du fichier original
    	remove("facture.txt");
	
    	// Renommage du fichier temporaire
    	rename("temp.txt", "facture.txt");
}	
