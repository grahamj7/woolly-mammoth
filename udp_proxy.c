/* CMPT 434 - Winter 2016
 * Assignment 1, Question 3
 *
 * Jordaen Graham - jhg257
 *
 * File: udp_proxy.c
 */

#include "server.h"
struct addrinfo *servinfo;
int server_fd = -1, proxy_fd = -1;

int udp_setup(char *host, char *port){
    struct addrinfo hints, *p;
    int server_fd = -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(host, port, &hints, &servinfo) != 0) {
        perror("getaddrinfo");
        exit(1);
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((server_fd = socket(p->ai_family, p->ai_socktype,
                                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        perror("talker: failed to create socket");
        exit(2);
    }

    return server_fd;
}

void *client_func(void *args){
    int nc_client_fd = *(int*)args;
    char *buffer;

    while(1) {
        buffer = recvTCPMessage(nc_client_fd);
        char *pos;
        if ((pos=strchr(buffer, '\n')) != NULL)
            *pos = '\0';
        if (strcmp(buffer, "quit") == 0)
            break;
        else if (strcmp(buffer, "abort") == 0) {
            break;
        }
		
		if ((sendto(server_fd, buffer, strlen(buffer)+1,
                    0, servinfo->ai_addr, servinfo->ai_addrlen)) == -1) {
            perror("talker: sendto");
            exit(1);
        }

        if (strcmp(buffer, "getall") == 0) {
            // if getall loop for responses
            char *message;
            size_t size;
            printf("Getall\n");
            while(1) {
                buffer = malloc((MAXDATASIZE - 1) * sizeof(char));
                bzero(buffer, MAXDATASIZE);
                struct sockaddr_storage their_addr;
                socklen_t addr_len;
                addr_len = sizeof their_addr;
                printf("Before: {%s}\n", buffer);
                if ((recvfrom(server_fd, buffer, MAXDATASIZE - 1,
                              0, (struct sockaddr *) &their_addr, &addr_len)) == -1) {
                    perror("recvfrom");
                    exit(1);
                }
                printf("Recv: {%s}\n", buffer);
                if (strcmp(buffer, "DONE") == 0)
                    break;
                strcat(buffer, "\n");
                printf("Send: {%s}\n", buffer);
                sendTCPMessage(nc_client_fd, buffer);
                free(buffer);
            }

            free(buffer);
        } else {
            // get Response
            buffer = malloc((MAXDATASIZE - 1) * sizeof(char));

            struct sockaddr_storage their_addr;
            socklen_t addr_len;
            addr_len = sizeof their_addr;

            if ((recvfrom(server_fd, buffer, MAXDATASIZE - 1,
                          0, (struct sockaddr *) &their_addr, &addr_len)) == -1) {
                perror("recvfrom");
                exit(1);
            }
            sendTCPMessage(nc_client_fd, buffer);
            free(buffer);
        }
    }

    close(nc_client_fd);
    pthread_exit(NULL);
}

void run_proxy_server(){
    int client_fd = -1;
    pthread_t client_thread;

    while(1){
        if ((client_thread = malloc(sizeof(pthread_t))) == 0){
            perror("create thread");
            exit(1);
        }

        if ((client_fd = get_connections(proxy_fd)) == -1) {
            continue;
        }

        if (pthread_create(&client_thread, NULL, client_func, &client_fd) == -1){
            perror("start pthread");
            exit(2);
        }

//        TODO
//        if (pthread_create(&client_thread, NULL, client_listener, &client_fd) == -1){
//            perror("start pthread");
//            exit(2);
//        }

    }
}

int main(int argc, char *argv[])
{
    char *host = "localhost", *port = PORT, *proxyport = PROXYPORT;

    if (argc >= 3) {
        host = argv[1];
        port = argv[2];
    }

    server_fd = udp_setup(host, port);
    proxy_fd = start(proxyport);
    run_proxy_server();

    close(server_fd);
    close(proxy_fd);
    freeaddrinfo(servinfo);

    return 0;
}

