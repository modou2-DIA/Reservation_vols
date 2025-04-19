CC = gcc
CFLAGS = -Wall -Iinclude

all: serveur client

serveur: src/main_server.c src/server.c src/utils.c
	$(CC) $(CFLAGS) -o serveur src/main_server.c src/server.c src/utils.c

client: src/main_client.c src/client.c src/utils.c
	$(CC) $(CFLAGS) -o client src/main_client.c src/client.c src/utils.c

clean:
	rm -f serveur client
