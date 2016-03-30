
#ifndef CMPT434_BARRIER_H
#define CMPT434_BARRIER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

int connect_to_server(char *);
int getConnections(int);
int initializeSocket(const char *);

#endif //CMPT434_BARRIER_H
