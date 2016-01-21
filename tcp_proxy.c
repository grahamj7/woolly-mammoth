/* CMPT 434 - Winter 2016
 * Assignment 1, Question 2
 *
 * Jordaen Graham - jhg257
 *
 * File: proxy.c
 */

#include "server.h"

int server_fd;

void *client_func(void *args){
    char *buffer;
    int client_fd;
    client_fd = *(int*) args;


    while(1) {
        buffer = recvMessage(client_fd);
        if (strcmp(buffer, "quit\n") == 0) {
            free(buffer);
            break;
        }
        else if (strcmp(buffer, "abort") == 0) {
            break;
        }

        sendMessage(server_fd, buffer);
        free(buffer);
        buffer = recvMessage(server_fd);
        sendMessage(client_fd, buffer);
        free(buffer);
    }

    close(client_fd);
    pthread_exit(NULL);
}

void run_proxy_server(int proxy_fd) {

    pthread_t client_thread;
    int client_fd;

    while(1) {  /* main accept() loop */
        /* Pointer to the location of all client thread */

        /* allocate space for the client_thread */
        client_thread = (pthread_t) malloc(sizeof(pthread_t));
        if (client_thread == 0){
            perror("create thread");
            pthread_exit(NULL);
        }

        client_fd = get_connections(proxy_fd);
        if (client_fd == -1) {
            continue;
        }

        if (pthread_create(&client_thread, NULL, client_func, &client_fd) == -1){
            perror("start pthread");
            pthread_exit(NULL);
        }
    }
}

int main(int argc, char **argv) {

    char *host = "localhost";
    char *port = PORT;

    if (3 <= argc) {
        host = argv[1];
        port = argv[2];
    } else {
        printf("Host and port not given, defaulting to %s:%s\n\n", host, port);
    }

    int proxy_fd = start(PROXYPORT);

    server_fd = connect_to_server(host, port);
    run_proxy_server(proxy_fd);

    return 0;
}

