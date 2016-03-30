
#include "barrier.h"

void sigchld_handler(int s){
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

/* get sockaddr, IPv4 or IPv6: */
void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int connect_to_server(char *port){
    int socket_fd = -1, rv;
    char s[INET6_ADDRSTRLEN];
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo("localhost", port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    /* loop through all the results and connect to the first we can */
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((socket_fd = socket(p->ai_family, p->ai_socktype,
                                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_fd);
            continue;
        }

        break;
    }
    if (p == NULL) {
        perror("client: failed to connect");
        exit(2);
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *) p->ai_addr),
              s, sizeof s);
    /*printf("client: connecting to %s\n", s); */

    freeaddrinfo(servinfo); /* all done with this structure */
    signal(SIGPIPE, SIG_IGN);
    return socket_fd;
}

/* Get TCP connections from other nodes */
int getConnections(int *serverSocket) {

    int clientSocket;
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];
    struct sockaddr_storage their_addr;
    sin_size = sizeof their_addr;

    clientSocket = accept(*serverSocket, (struct sockaddr *)&their_addr, &sin_size);
    if (clientSocket == -1) {
        perror("accept");
        fprintf(stderr, "Socket: %d_%d\n", *serverSocket, clientSocket);
        return clientSocket;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), (char *) s, INET6_ADDRSTRLEN);

//    printf("\nserver: got connection from %s\n", s);
    fflush(stdout);


    return clientSocket;
}

int initializeSocket(const char *port){
    struct addrinfo hints, *servinfo, *p;
    struct sigaction sa;

    int rv, yes = 1, serverSocket = 0;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; /* use my IP */

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }


    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((serverSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(serverSocket, p->ai_addr, p->ai_addrlen) == -1) {
            perror("server node: bind");
            printf("%s\n", port);
            fflush(stdout);
            continue;
        }

        break;
    }

    if (listen(serverSocket, 10) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; /* reap all dead processes */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    return serverSocket;
}
