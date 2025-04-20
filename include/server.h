
//éviter que le fichier soit inclus plusieurs fois dans un même programme
#ifndef SERVER_H
#define SERVER_H

#define MAX_VOLS 100
#define MAX_AGENCES 10
#define PORT 8080

typedef struct {
    char reference[10];
    char destination[50];
    int places_disponibles;
    float prix_place;
} Vol;

typedef struct {
    char reference_vol[10];
    int agence;
    char transaction[20]; // "Demande" ou "Annulation"
    int valeur;
    char resultat[20]; // "succès" ou "impossible"
} Transaction;

//cette structure est utilisée pour passer plusieurs paramètres à un thread
typedef struct {
    int socket_agence; // le descripteur du socket connecté à l'agence
    Vol* vols;
    int nb_vols;
    pthread_mutex_t* mutex_vols; //pointeur vers un mutex (verrou) pour synchroniser l’accès au fichier de vols
    pthread_mutex_t* mutex_histo; // idem 
} ThreadData;

// Fonctions de gestion des vols

//1.Lit les vols à partir d’un fichier texte et retourne un tableau dynamique de Vol
Vol* charger_vols(const char* filename, int* nb_vols);

//2.mettre à jour les données de vols après une réservation ou annulation
int mettre_a_jour_vols(const char* filename, Vol* vols, int nb_vols);

//3.Vérifie s’il reste suffisamment de places dans le vol ref_vol pour satisfaire une demande
int verifier_disponibilite(Vol* vols, int nb_vols, const char* ref_vol, int places);

// Fonctions de gestion des transactions
int ajouter_transaction_historique(const char* filename, Transaction* trans);

// Fonctions réseau
//Cette fonction est le corps d’un thread
void* gestion_agence(void* socket_desc);

#endif
