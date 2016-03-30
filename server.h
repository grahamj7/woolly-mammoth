/* CMPT 434 - Winter 2016
 * Assignment 2, Question 1
 *
 * Jordaen Graham - jhg257
 *
 * File: server.h
 */


#ifndef CMPT434_SERVER_H
#define CMPT434_SERVER_H

#include <pthread.h>
#include <sys/mman.h>
#include <math.h>
#include "connections.h"

#define STARTPORT "31000"	/* the port users will be connecting to */
#define NumTags 3
#define GridSize 1001
#define BUFFSIZE 1000

int Dist, NumSteps, Range, Packets, Output, Delta;

struct packet_buffer {
    char* packet;
    int id;
    struct packet_buffer *next;
};

struct nodes {
    int id, x, y, p_sockets[NumTags], has[NumTags], delete[NumTags], numRemaining, my_socket, packet_count;
    char *name, *port, *p_ports[NumTags];
    struct packet_buffer *packets;
};

struct both {
    int socket;
    struct nodes *tag;
} *both1[NumTags];

static struct nodes *baseStation, **animal_tags;


#endif /* CMPT434_SERVER_H */
