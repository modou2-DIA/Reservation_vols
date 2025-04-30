#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "../include/server.h"

void start_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    // Initialisation des mutex
    pthread_mutex_t mutex_vols = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_histo = PTHREAD_MUTEX_INITIALIZER;

    // Chargement des vols
    int nb_vols;
    Vol* vols = charger_vols("data/vols.txt", &nb_vols);
    if (vols == NULL) exit(EXIT_FAILURE);

    // Création du socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configuration du socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en écoute sur le port %d...\n", PORT);

    // Accepter les connexions
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept");
            continue;
        }

        // Préparation des données pour le thread
        ThreadData* thread_data = malloc(sizeof(ThreadData));
        thread_data->socket_agence = new_socket;
        thread_data->vols = vols;
        thread_data->nb_vols = nb_vols;
        thread_data->mutex_vols = &mutex_vols;
        thread_data->mutex_histo = &mutex_histo;

        // Création du thread
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, gestion_agence, thread_data) != 0) {
            perror("Erreur création thread");
            close(new_socket);
            free(thread_data);
        }
    }

    free(vols);
    close(server_fd);
}

int main() {
    start_server();
    return 0;
}
