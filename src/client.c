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
    // Logique de calcul (à compléter)
    printf("Facture générée pour l'agence %d\n", agence_id);
}
