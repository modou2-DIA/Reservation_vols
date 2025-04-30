#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/client.h"  // Inclure les constantes

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <ID_AGENCE>\n", argv[0]);
        return -1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT); // Port depuis client.h

    // IP fixe depuis client.h
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("Adresse IP invalide\n");
        return -1;
    }

    // Connexion
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connexion échouée\n");
        return -1;
    }

    // Logique de transaction
    Transaction trans;
    saisir_transaction(&trans, atoi(argv[1]));
    send(sock, &trans, sizeof(trans), 0);

    char buffer[1024];
    read(sock, buffer, sizeof(buffer));
    afficher_resultat(buffer);

    close(sock);
    return 0;
}
