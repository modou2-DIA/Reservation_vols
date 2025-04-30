#ifndef CLIENT_H
#define CLIENT_H

#include "../include/server.h"

#define SERVER_IP "127.0.0.1"  // IP fixe
#define SERVER_PORT 8080        // Port fixe

void saisir_transaction(Transaction* trans, int agence_id);
void afficher_resultat(const char* result);
void calculer_facture(int agence_id);

#endif
