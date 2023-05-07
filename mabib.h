typedef struct
{	int ref_vol;
	char destination[30];
	int place_dispo;
	float prix_place;
}VOL;
//fonctions de la structure vol
void saisir_vol(VOL*);
void afficher_vol(VOL);
void ecrire_vol_fichier(VOL);
VOL afficher_vol_fichier(int);
int reserver(int,int,int);
int annuler(int,int,int);
///////////////////////////////////////////////////
typedef struct
{	int ref_vol;
	int agence;
	char transaction[30];
	int valeur;
	char resultat[30];
}RESERVATION;
//fonctions de la structure reservation
void saisir_res(RESERVATION*);
void afficher_res(RESERVATION);
void ecrire_res_fichier(RESERVATION);
void afficher_res_fichier();
///////////////////////////////////////////////////
typedef struct
{	int ref_agence;
	float somme;
}FACTURE;
//fonctions de la structure facture
void saisir_fac(FACTURE*);
void afficher_fac(FACTURE);
void ecrire_fac_fichier(FACTURE);
FACTURE afficher_fac_fichier(int);
void modifier_fac_fichier(int,float);