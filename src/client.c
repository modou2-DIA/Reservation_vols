#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/client.h"

void saisir_transaction(Transaction* trans, int agence_id) {
    printf("Référence vol: ");
    scanf("%s", trans->reference_vol);
    printf("Nombre de places: ");
    scanf("%d", &trans->valeur);
    printf("Type (Demande/Annulation): ");
    scanf("%s", trans->transaction);
    trans->agence = agence_id;
}

void afficher_resultat(const char* result) {
    printf("Résultat: %s\n", result);
}

void calculer_facture(int agence_id) {
// Ouverture du fichier historique des transactions
    FILE *file = fopen("data/historique.txt", "r");
    if (!file) {
        perror("Erreur ouverture fichier historique");
        return;
    }
    
// Ouverture du fichier des vols pour obtenir les prix
    FILE *vols_file = fopen("data/vols.txt", "r");
    if (!vols_file) {
        perror("Erreur ouverture fichier vols");
        fclose(file);
        return;
    }

    float total = 0.0;
    char ligne[256];
    
     // Ignorer la ligne d'en-tête
    fgets(ligne, sizeof(ligne), file);
    
    
// Parcourir toutes les transactions
    while (fgets(ligne, sizeof(ligne), file)) {
        char ref_vol[10], transaction[20], resultat[20];
        int agence, places;

        sscanf(ligne, "%s %d %s %d %s", 
               ref_vol, &agence, transaction, &places, resultat);
               
        // Ne traiter que les transactions réussies de cette agence
        if (agence == agence_id && strcmp(resultat, "succès") == 0) {
            // Trouver le prix du vol correspondant
            rewind(vols_file);
            char vol_ref[10], destination[50];
            int nb_places;
            float prix;
            
            // Passer l'en-tête vols
            fgets(ligne, sizeof(ligne), vols_file);
            
            while (fgets(ligne, sizeof(ligne), vols_file)) {
                sscanf(ligne, "%s %s %d %f", vol_ref, destination, &nb_places, &prix);
                if (strcmp(vol_ref, ref_vol) == 0) {
                    if (strcmp(transaction, "Demande") == 0) {
                        total += places * prix;
                    } else if (strcmp(transaction, "Annulation") == 0) {
                        // Pénalité de 10% sur le montant remboursé
                        total -= places * prix * 0.9;
                    }
                    break;
                }
            }
        }
    }

    fclose(file);
    fclose(vols_file);

    // Mise à jour du fichier facture.txt
    FILE *facture_file = fopen("data/facture.txt", "r+");
    if (!facture_file) {
         // Si le fichier n'existe pas, on le crée
        facture_file = fopen("data/facture.txt", "w");
        fprintf(facture_file, "Référence Agence Somme à payer\n");
    }

    // Lire et mettre à jour les factures existantes
    char temp_file[] = "data/facture_temp.txt";
    FILE *temp = fopen(temp_file, "w");
    fprintf(temp, "Référence Agence Somme à payer\n");
    
    // Copie des anciennes factures avec mise à jour
    int found = 0; //pour savoir si l'agence existait déjà
    rewind(facture_file);
    fgets(ligne, sizeof(ligne), facture_file); // Passer l'en-tête
    
    while (fgets(ligne, sizeof(ligne), facture_file)) {
        int current_agence;
        float montant;
        
        if (sscanf(ligne, "%d %f", &current_agence, &montant) == 2) {
            if (current_agence == agence_id) {
                 // Mise à jour de la facture existante
                fprintf(temp, "%d %.2f\n", agence_id, total);
                found = 1;
            } else {
                // Recopier les autres factures inchangées
                fprintf(temp, "%d %.2f\n", current_agence, montant);
            }
        }
    }
    
    // Si l'agence n'avait pas encore de facture
    if (!found) {
        fprintf(temp, "%d %.2f\n", agence_id, total);
    }
    
    fclose(facture_file);
    fclose(temp);
    
     // Remplacer l'ancien fichier par le nouveau
    remove("data/facture.txt");
    rename(temp_file, "data/facture.txt");
    
    printf("Facture générée pour l'agence %d: %.2f\n", agence_id, total);
}
