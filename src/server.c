#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../include/server.h"

Vol* charger_vols(const char* filename, int* nb_vols) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier des vols");
        return NULL;
    }

    Vol* vols = malloc(sizeof(Vol) * MAX_VOLS);
    *nb_vols = 0;

    while (fscanf(file, "%s %s %d %f",
                  vols[*nb_vols].reference,
                  vols[*nb_vols].destination,
                  &vols[*nb_vols].places_disponibles,
                  &vols[*nb_vols].prix_place) == 4) {
        (*nb_vols)++;
        if (*nb_vols >= MAX_VOLS) break;
    }

    fclose(file);
    return vols;
}

// Mettre à jour le fichier de vols
int mettre_a_jour_vols(const char* filename, Vol* vols, int nb_vols) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier pour mise à jour");
        return -1;
    }

    for (int i = 0; i < nb_vols; i++) {
        fprintf(file, "%s %s %d %.2f\n",
                vols[i].reference,
                vols[i].destination,
                vols[i].places_disponibles,
                vols[i].prix_place);
    }

    fclose(file);
    return 0;
}

// Vérifier la disponibilité d'un vol
int verifier_disponibilite(Vol* vols, int nb_vols, const char* ref_vol, int places) {
    for (int i = 0; i < nb_vols; i++) {
        if (strcmp(vols[i].reference, ref_vol) == 0) {
            return vols[i].places_disponibles >= places;
        }
    }
    return 0; // Vol non trouvé
}

// Ajouter une transaction à l'historique
int ajouter_transaction_historique(const char* filename, Transaction* trans) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier d'historique");
        return -1;
    }

    fprintf(file, "%s %d %s %d %s\n",
            trans->reference_vol,
            trans->agence,
            trans->transaction,
            trans->valeur,
            trans->resultat);

    fclose(file);
    return 0;
}


void* gestion_agence(void* arg) {
    ThreadData* data = (ThreadData*)arg; //cast
    char buffer[1024]; //pour lire la requête du client.
    
    // Lire la requête du client 
    read(data->socket_agence, buffer, sizeof(buffer));
    
    // Traiter la requête (ex: vérifier disponibilité)
    pthread_mutex_lock(data->mutex_vols);
    int disponible = verifier_disponibilite(data->vols, data->nb_vols, "1000", 5);// vérifier si le vol 1000 a 5 places disponibles.
    pthread_mutex_unlock(data->mutex_vols);//On libère le mutex,
    
    // Répondre au client
    write(data->socket_agence, disponible ? "SUCCES" : "ECHEC", 6); //6 = taille maximale des deux mots
    
    close(data->socket_agence); // on ferme la socket agence
    free(data); //On libère la mémoire allouée pour ThreadData
    pthread_exit(NULL); //On termine le thread
}


