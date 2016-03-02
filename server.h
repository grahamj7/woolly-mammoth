/* CMPT 434 - Winter 2016
 * Assignment 2, Question 1
 *
 * Jordaen Graham - jhg257
 *
 * File: server.h
 */


#ifndef CMPT434_SERVER_H
#define CMPT434_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>

struct message{
    char *text;
    int sequence_num;
};

struct message_queue{
    char *text;
    struct message_queue *next;
};

#define PORT "31950"	// the port users will be connecting to
#define WINDOW_SIZE 5 // the port users will be connecting to
#define MAXBUFLEN 1000
#define MAXSEQUENCE 256

#endif //CMPT434_SERVER_H
