#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/client.h"

void afficher_menu() {
    printf("\n=== Menu Agence ===\n");
    printf("1. Effectuer une réservation\n");
    printf("2. Annuler une réservation\n");
    printf("3. Calculer ma facture\n");
    printf("4. Quitter\n");
    printf("Choix: ");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <ID_AGENCE>\n", argv[0]);
        return -1;
    }

    int agence_id = atoi(argv[1]);
    char choix;
    
    do {
        afficher_menu();
        scanf(" %c", &choix);
        
        switch(choix) {
            case '1':
            case '2': {
                // Réutilisation directe de saisir_transaction()
                Transaction trans;
                saisir_transaction(&trans, agence_id);
                
                // Configuration de la connexion
                int sock = socket(AF_INET, SOCK_STREAM, 0);
                struct sockaddr_in serv_addr = {
                    .sin_family = AF_INET,
                    .sin_port = htons(SERVER_PORT)
                };
                
                if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
                    printf("Adresse IP invalide\n");
                    break;
                }

                if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
                    printf("Connexion échouée\n");
                    break;
                }

                // Envoi et réception
                send(sock, &trans, sizeof(trans), 0);
                
                char buffer[1024];
                read(sock, buffer, sizeof(buffer));
                
                // Réutilisation de afficher_resultat()
                afficher_resultat(buffer);
                
                close(sock);
                break;
            }
                
            case '3':
                // Réutilisation directe de calculer_facture()
                calculer_facture(agence_id);
                break;
                
            case '4':
                printf("Au revoir!\n");
                break;
                
            default:
                printf("Choix invalide!\n");
        }
        
    } while(choix != '4');

    return 0;
}
