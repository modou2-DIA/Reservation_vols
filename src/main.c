#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/server.h"

void afficher_vols(Vol* vols, int nb_vols) {
    printf("Liste des vols disponibles :\n");
    printf("Référence\tDestination\tPlaces\tPrix\n");
    for (int i = 0; i < nb_vols; i++) {
        printf("%s\t\t%s\t\t%d\t%.2f\n", 
               vols[i].reference,
               vols[i].destination,
               vols[i].places_disponibles,
               vols[i].prix_place);
    }
    printf("\n");
}

void tester_disponibilite(Vol* vols, int nb_vols, const char* ref_vol, int places) {
    printf("Test disponibilité : vol %s, %d places... ", ref_vol, places);
    if (verifier_disponibilite(vols, nb_vols, ref_vol, places)) {
        printf("DISPONIBLE\n");
    } else {
        printf("INDISPONIBLE\n");
    }
}

void simuler_transaction(Vol* vols, int nb_vols, const char* filename_histo, const char* ref_vol, int agence, const char* type_trans, int places) {
    printf("\nSimulation transaction :\n");
    printf("- Agence: %d\n", agence);
    printf("- Type: %s\n", type_trans);
    printf("- Vol: %s\n", ref_vol);
    printf("- Places: %d\n", places);
    
    Transaction trans;
    strcpy(trans.reference_vol, ref_vol);
    trans.agence = agence;
    strcpy(trans.transaction, type_trans);
    trans.valeur = places;
    
    if (strcmp(type_trans, "Demande") == 0) {
        if (verifier_disponibilite(vols, nb_vols, ref_vol, places)) {
            // Mettre à jour les places disponibles
            for (int i = 0; i < nb_vols; i++) {
                if (strcmp(vols[i].reference, ref_vol) == 0) {
                    vols[i].places_disponibles -= places;
                    break;
                }
            }
            strcpy(trans.resultat, "succès");
            printf("Réservation réussie!\n");
        } else {
            strcpy(trans.resultat, "impossible");
            printf("Réservation impossible: places insuffisantes\n");
        }
    } else if (strcmp(type_trans, "Annulation") == 0) {
        // Pour l'annulation, on ajoute les places
        for (int i = 0; i < nb_vols; i++) {
            if (strcmp(vols[i].reference, ref_vol) == 0) {
                vols[i].places_disponibles += places;
                break;
            }
        }
        strcpy(trans.resultat, "succès");
        printf("Annulation réussie!\n");
    }
    
    // Ajouter à l'historique
    if (ajouter_transaction_historique(filename_histo, &trans) == 0) {
        printf("Transaction enregistrée dans l'historique.\n");
    } else {
        printf("Erreur lors de l'enregistrement de la transaction.\n");
    }
}

int main() {

// les chemins des fichiers
    const char* filename_vols = "data/vols.txt";
    const char* filename_histo = "data/histo.txt";
    
    // 1. Test de chargement des vols
    printf("=== Test 1: Chargement des vols ===\n");
    int nb_vols;
    Vol* vols = charger_vols(filename_vols, &nb_vols);
    
    if (vols == NULL || nb_vols == 0) {
        printf("Erreur: aucun vol chargé\n");
        return 1;
    }
    
    afficher_vols(vols, nb_vols);
    
    // 2. Test de vérification de disponibilité
    printf("\n=== Test 2: Vérification disponibilité ===\n");
    tester_disponibilite(vols, nb_vols, "1000", 5);   // Doit réussir (20 dispo)
    tester_disponibilite(vols, nb_vols, "1000", 25);  // Doit échouer
    tester_disponibilite(vols, nb_vols, "2000", 10);  // Doit réussir
    tester_disponibilite(vols, nb_vols, "5000", 5);   // Vol inexistant
    
    // 3. Test de transactions
    printf("\n=== Test 3: Simulation de transactions ===\n");
    
    // Transaction 1: Réservation réussie
    simuler_transaction(vols, nb_vols, filename_histo, "1000", 1, "Demande", 5);
    afficher_vols(vols, nb_vols);
    
    // Transaction 2: Réservation impossible
    simuler_transaction(vols, nb_vols, filename_histo, "2000", 2, "Demande", 15);
    afficher_vols(vols, nb_vols);
    
    // Transaction 3: Annulation
    simuler_transaction(vols, nb_vols, filename_histo, "1000", 1, "Annulation", 3);
    afficher_vols(vols, nb_vols);
    
    // 4. Test de mise à jour du fichier vols
    printf("\n=== Test 4: Mise à jour fichier vols ===\n");
    if (mettre_a_jour_vols(filename_vols, vols, nb_vols) == 0) {
        printf("Fichier vols mis à jour avec succès.\n");
        
        // Recharger pour vérifier
        Vol* vols_recharges = charger_vols(filename_vols, &nb_vols);
        printf("\nContenu après rechargement:\n");
        afficher_vols(vols_recharges, nb_vols);
        free(vols_recharges);
    } else {
        printf("Erreur lors de la mise à jour du fichier vols.\n");
    }
    
    // 5. Vérification fichier historique
    printf("\n=== Test 5: Vérification fichier historique ===\n");
    printf("Le fichier histo.txt devrait contenir 3 nouvelles transactions.\n");
    printf("Veuillez vérifier manuellement son contenu.\n");
    
    // Nettoyage
    free(vols);
    
    return 0;
}
