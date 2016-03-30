/* CMPT 434 - Winter 2016
 * Assignment 3, Question 1
 *
 * Jordaen Graham - jhg257
 *
 * File: sender.c
 */

#include "server.h"

void move(struct nodes *tag) {
//      move a distance, Dist, within the simulated region;
//          if the boundary of the region is hit, bounce off so that the total distance D is still
//        covered while remaining within the region
    int x = tag->x, y = tag->y;
    int dir = rand()%8;
    switch(dir) {
        case 0:
//            N
            if (x < Dist)
                x = Dist - x;
            else
                x = x - Dist;
            break;
        case 1:
//            NE
            if (x < Dist)
                x = Dist - x;
            else
                x = x - Dist;
            if ((y+Dist) >= GridSize)
                y = ((GridSize-1) - ((Dist + y) - (GridSize-1)));
            else
                y = y + Dist;
            break;
        case 2:
//            E
            if ((y+Dist) >= GridSize)
                y = ((GridSize-1) - ((Dist + y) - (GridSize-1)));
            else
                y = y + Dist;
            break;
        case 3:
//            SE
            if ((x+Dist) >= GridSize)
                x = ((GridSize-1) - ((Dist + x) - (GridSize-1)));
            else
                x = x + Dist;
            if ((y+Dist) >= GridSize)
                y = ((GridSize-1) - ((Dist + y) - (GridSize-1)));
            else
                y = y + Dist;
            break;
        case 4:
//            S
            if ((x+Dist) >= GridSize)
                x = ((GridSize-1) - ((Dist + x) - (GridSize-1)));
            else
                x = x + Dist;
            break;
        case 5:
//            SW
            if ((x+Dist) >= GridSize)
                x = ((GridSize-1) - ((Dist + x) - (GridSize-1)));
            else
                x = x + Dist;
            if (y < Dist)
                y = Dist - y;
            else
                y = y - Dist;
            break;
        case 6:
//            W
            if (y < Dist)
                y = Dist - y;
            else
                y = y - Dist;
            break;
        case 7:
//            NW
            if (x < Dist)
                x = Dist - x;
            else
                x = x - Dist;
            if (y < Dist)
                y = Dist - y;
            else
                y = y - Dist;
            break;
        default:break;
    };
    tag->x = x;
    tag->y = y;
}

int sendMessage(int i, struct nodes *tag) {
    char *message;
    int socket;
    if (i == tag->id) {
        printf("\tId: %d_B, Port: %s\n", tag->id, tag->p_ports[i]);
    } else {
        printf("\tId: %d_%d, Port: %s\n", tag->id, i, tag->p_ports[i]);
    }
    if (i == tag->id) {return -1;}  // TODO
    socket = connect_to_server(tag->p_ports[i]);

    message = malloc(sizeof(char)*(BUFFSIZE));
    sprintf(message,"|%s|%d|%d|%d|%d|", tag->name, tag->id, tag->x, tag->y, i);
    if (send(socket, message, strlen(message)+1, 0) == -1)
        fprintf(stdout, "sendMessage Error: sending to %d\n", socket);
    return socket;
}

int receiveNumPackets(int id, int socket) {
    int packets, dstID;
    char *buffer;

    buffer = malloc(sizeof(char)*BUFFSIZE);
    if(recv(socket, buffer, BUFFSIZE, 0) == -1)
        fprintf(stdout, "recvMessage Error: receiving from %d\n", socket);
//    printf("%d Received Response: %s\n", id, buffer);
    packets = atoi(strtok(buffer,"|"));
    dstID = atoi(strtok(NULL,"|"));
//    printf("P: %d, ID: %d, Min: %d\n", packets, dstID, min(packets, Packets));

    return packets;
}

void receivePackets(struct nodes *tag, int packets, int socket) {
    char *buffer;
    for (int j=0; j<packets; j++) {
        // Recv packets
        buffer = malloc(sizeof(char)*BUFFSIZE);
        if(recv(socket, buffer, BUFFSIZE, 0) == -1)
            fprintf(stdout, "recvMessage Error: receiving packet from %d\n", socket);
//        printf("%d Received Packet: %s\n", tag->id, buffer);

        // add packets to buffer
        struct packet_buffer *packet = malloc(sizeof(struct packet_buffer));
        packet->next=NULL;
        packet->packet = malloc(sizeof(char)*(strlen(buffer)+1));
        strcpy(packet->packet, buffer);
        if (tag->packets != NULL) {packet->next = tag->packets;}
        tag->packets = packet;
        tag->count_packets += 1;
        free(buffer);
    }
}

void exchangePackets(struct nodes *tag) {
    int socket, numPackets;
    for (int i=0; i<NumTags; i++) {
        socket = sendMessage(i, tag);
        if (socket == -1) continue;  // TODO
        numPackets = receiveNumPackets(tag->id, socket);
        if (numPackets > 0)
            receivePackets(tag, numPackets, socket);
    }
}

int in_range(int srcX, int srcY, struct nodes *dst) {
//    printf("(%d,%d) - (%d,%d)\n", dst->x, dst->y, srcX, srcY);
    double x = pow(abs(dst->x - srcX), 2);
    double y = pow(abs(dst->y - srcY), 2);
    if (sqrt(x+y)<=(double)Range)
        return 1;
    return 0;
}

void response(int client_fd, int id, int packets) {
    char *message = malloc(sizeof(char)*BUFFSIZE);
    sprintf(message, "%d|%d", min(Packets, packets), id); // Num packets in buffer max of Packets
//    printf("Send Response: %s\n", message);
    if (send(client_fd, message, strlen(message)+1, 0) == -1) {
        fprintf(stdout, "sendMessage Error: sending to %d\n", client_fd);
    }
    free(message);
    // TODO send numPackets
}

void *listenerThread(void *args) {
    struct both *both1 = (struct both*)args;
    struct nodes *tag = both1->tag;
    int *my_socket = &both1->socket, srcID, dstID;
    int srcX, srcY, num_packets=0;
    char *buffer = malloc(sizeof(char)*BUFFSIZE);
    char *srcName = malloc(sizeof(char)*BUFFSIZE);

//    printf("Id: %d, Socket: %d\n", tag->id, *my_socket);
    int clientSocket = getConnections(my_socket);
    if (clientSocket == -1) {
        close(*my_socket);
        return (void*)-1;
    }

    ssize_t numbytes;
    if ((numbytes = recv(clientSocket, buffer, BUFFSIZE, 0)) == -1) {
        perror("recv");
        exit(0);
    }
    else{
        buffer[numbytes] = '\0';
    }
//    printf("Listener Received: %s\n", buffer);

    strcpy(srcName, strtok(buffer,"|"));
    srcID = atoi(strtok(NULL,"|"));
    srcX = atoi(strtok(NULL,"|"));
    srcY = atoi(strtok(NULL,"|"));
    dstID = atoi(strtok(NULL,"|"));

    struct nodes *src=animal_tags[srcID];
    src->x = srcX;
    src->y = srcY;

//    printf("Verify: %d==%d\n", dstID, tag->id);

    if (in_range(srcX, srcY, tag)) {
//        printf("In range\n");
//        num_packets=;
    }
    response(clientSocket, dstID, num_packets);

    free(buffer);
    free(srcName);
    close(clientSocket);
    pthread_exit(NULL);
}

void start_simulation(struct nodes *tag_info) {
    pthread_t listen_thread;

    for(int j=0; j<NumTags; j++){
        if (j == tag_info->id) {
            tag_info->p_ports[j] = baseStation->p_ports[tag_info->id];
//            printf("Id: %d_B, Port: %s, Base: %s\n", tag_info->id, tag_info->p_ports[j], baseStation->p_ports[tag_info->id]);
            continue;
        }
        tag_info->p_ports[j] = malloc(sizeof(char)*10);
        sprintf(tag_info->p_ports[j], "%d", atoi(tag_info->port)+j);
        tag_info->p_sockets[j] = initializeSocket(tag_info->p_ports[j]);
//        printf("Id: %d_%d, Port: %s, Sock: %d\n", tag_info->id, j, tag_info->p_ports[j], tag_info->p_sockets[j]);
    }
    for (int s=0; s<NumSteps; s++) {
        for(int i = 0; i<NumTags; i++){
            if (i == tag_info->id) {continue;}
            both1[i] = malloc(sizeof(struct both));
            both1[i]->tag = tag_info;
            both1[i]->socket = tag_info->p_sockets[i];
            pthread_create(&listen_thread, NULL, listenerThread, both1[i]);
        }
        move(tag_info);
        exchangePackets(tag_info);

        // barrier
        sleep(1);
    }
    for (int i=0; i<NumTags; i++)
        free(both1[i]);
}

void run_baseStation() {

//    Send message
//    receive numPackets
//    receive packets
//    if Output, print everytime a packet is received
//    count the number of packets received

//    end; print count
}

int main(int argc, char *argv[]) {
    char *port=STARTPORT, *s_port;
    /* Get command line args */

//    if(argc < 6) {
//        printf("\nusage: ./delayTolerant <Steps> <Distance> <Range> <Packets> <Output>\n");
//        return 1;
//    }
//    NumSteps = atoi(argv[1]);
//    Dist = atoi(argv[2]);
//    Range = atoi(argv[3]);
//    Packets = atoi(argv[4]);
//    Output = atoi(argv[5]);

    srand(NULL);
    /* Create BaseStation */
    baseStation = malloc(sizeof(struct nodes));
    baseStation->id = -1;
    baseStation->name = "N00";
    baseStation->x = (GridSize-1)/2;
    baseStation->y = (GridSize-1)/2;
    baseStation->port = malloc(sizeof(char)*10);
    sprintf(baseStation->port, "%d", atoi(port)+1500);
    for(int i=0; i < NumTags; i++){
        baseStation->p_ports[i] = malloc(sizeof(char)*10);
        sprintf(baseStation->p_ports[i], "%d", atoi(baseStation->port)+i);
        baseStation->p_sockets[i] = initializeSocket(baseStation->p_ports[i]);
//        printf("Id: B_%d, Port: %s, Sock: %d\n", i, baseStation->p_ports[i], baseStation->p_sockets[i]);
    }

    animal_tags = mmap(NULL, NumTags, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    animal_tags = malloc(NumTags*sizeof(struct nodes*));

    /* Create Animal Tag Nodes */
    for (int i=0; i<NumTags; i++) {
        animal_tags[i] = malloc(sizeof(struct nodes));
        animal_tags[i]->id = i;
        animal_tags[i]->name = malloc(sizeof(char)*4);
        if ((i+1)<10)
            snprintf(animal_tags[i]->name, 4, "N0%d", i+1);
        else
            snprintf(animal_tags[i]->name, 4, "N%d", i+1);
        animal_tags[i]->x = rand()%(GridSize-1);
        animal_tags[i]->y = rand()%(GridSize-1);

        animal_tags[i]->port = malloc(sizeof(char)*10);
        sprintf(animal_tags[i]->port, "%d", atoi(STARTPORT)+(i*100));
        animal_tags[i]->count_packets = 0;
        struct packet_buffer *my_packet = malloc(sizeof(struct packet_buffer));
        my_packet->next = NULL;
        my_packet->packet = malloc(sizeof(char)*20);
        snprintf(my_packet->packet, 20, "My Data packet_%s", animal_tags[i]->name);
        animal_tags[i]->packets = my_packet;
    }
    /* Start the animal tags */
    for (int i=0; i<NumTags; i++) {
        if (fork() == 0) {
            // Child Process
            start_simulation(animal_tags[i]);
            exit(0);
        }
    }
    /* Run the base station */
    run_baseStation();

    while ((wait(NULL)) > 0);
    /* Cleanup */
    for(int j=0; j<NumTags; j++) {
        close(baseStation->p_sockets[j]);
        free(baseStation->p_ports[j]);
    }
    free(baseStation->packets);
    free(baseStation->port);
    free(baseStation);

    for (int i=0; i<NumTags; i++) {
        for(int j=0; j<NumTags; j++) {
            close(animal_tags[i]->p_sockets[j]);
        }
        free(animal_tags[i]->packets->packet);
        free(animal_tags[i]->packets);
        free(animal_tags[i]->port);
        free(animal_tags[i]->name);
        free(animal_tags[i]);
    }
    return 0;
}
