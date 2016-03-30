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
    char *message=malloc(sizeof(char)*(BUFFSIZE)), *has=malloc(sizeof(char)*BUFFSIZE);
    int socket;

    socket = connect_to_server(animal_tags[i]->p_ports[tag->id]);

    for(int j=0; j<NumTags; j++)
        if (tag->has[j]==1)
            sprintf(has, "%s,%d", has, j);

    sprintf(message,"|-1|%s|%d|%d|%d|%d|{%s}|\0", tag->name, tag->id, tag->x, tag->y, i, has);
    printf("Send: %s -> %s\n", animal_tags[i]->p_ports[tag->id], message);
    if (send(socket, message, strlen(message)+1, 0) == -1)
        fprintf(stdout, "sendMessage Error: sending to %d\n", socket);
    free(has);
    free(message);
    return socket;
}

void sendPackets(struct nodes *tag) {
    printf("Send Packets: %d -> B\n", tag->id);
}

void receiveNumPackets(struct nodes *tag, int socket) {
    int packets, numbytes, type;
    char *buffer, *data_packet, *temp;
    temp=malloc(sizeof(char)*BUFFSIZE);
    numbytes = (int) recv(socket, temp, BUFFSIZE, MSG_PEEK);
    free(temp);
    buffer = malloc(sizeof(char) * numbytes);
    if (recv(socket, buffer, (size_t) numbytes, 0) == -1) {
        printf("recv error");
        exit(1);
    }
    type = atoi(strtok(buffer, "|"));
    if (type == -2)
        sendPackets(tag);

    packets = atoi(strtok(NULL, "|"));
    data_packet = malloc(sizeof(char)*numbytes);
    strcpy(data_packet, strtok(NULL, "|"));

    if (packets == 0)
        return;

    int id, j=0;
    char *data, *data_list[packets], *temp_data;
    temp_data=malloc(sizeof(char)*numbytes);


    temp_data = strtok(data_packet, "{");
    while ( temp_data != NULL) {
        data_list[j] = malloc(sizeof(char)*strlen(temp_data));
        snprintf(data_list[j], strlen(temp_data), "%s", temp_data);
        data_list[j][strlen(temp_data)] = '\0';

        j++;
        temp_data = strtok(NULL, "{");
    }

    for (j=0;j<packets;j++) {
        id = atoi(strtok(data_list[j], "="));
        data = malloc(sizeof(char)*18);
        strcpy(data, strtok(NULL, "="));
        tag->has[id] = 1;

        // add packets to buffer
        struct packet_buffer *packet = malloc(sizeof(struct packet_buffer));
        packet->next = NULL;
        packet->id = id;
        packet->packet = malloc(sizeof(char)*18);
        sprintf(packet->packet, "%s\0", data);
        if (tag->packets != NULL) { packet->next = tag->packets; }
        tag->packets = packet;

        free(data);
    }

    free(temp_data);
    free(data_packet);
    free(buffer);
}

void exchangePackets(struct nodes *tag) {
    int socket, numPackets;
    for (int i=0; i<NumTags; i++) {
        socket = sendMessage(i, tag);
        if (socket == -1) continue;  // TODO
        receiveNumPackets(tag, socket);
        close(socket);
    }
}

int compute_closer(int x1, int y1, int x2, int y2) {
    double x_1 = pow(abs(baseStation->x - x1), 2);
    double y_1 = pow(abs(baseStation->y - y1), 2);
    double x_2 = pow(abs(baseStation->x - x2), 2);
    double y_2 = pow(abs(baseStation->y - y2), 2);
    if ((x_1+y_1) < ((x_2+y_2) - Delta))
        return 1;
    return 0;
}

int in_range(int srcX, int srcY, struct nodes *dst) {
    double x = pow(abs(srcX - dst->x), 2);
    double y = pow(abs(srcY - dst->y), 2);

    if((x + y) <= pow(Range,2))
        return compute_closer(srcX, srcY, dst->x, dst->y);
    return 0;
}

void response(int type, int client_fd, int id, struct nodes *tag, char *has) {
    char *message, *temp;
    int count=0;

    if (tag != NULL && tag->numRemaining > 0) {
        int has_list[NumTags]={0,0,0,0,0,0,0,0,0,0}, in;
        char *c_index = malloc(sizeof(char));
        strtok(has, ",");
        while ((c_index = strtok(NULL, ",")) != NULL) {
            in = atoi(c_index);
            has_list[in] = 1;
        }
        free(c_index);
        temp = malloc(sizeof(char)*BUFFSIZE);
        bzero(temp, BUFFSIZE);
        struct packet_buffer *cursor = tag->packets;
        while(cursor != NULL) {
            if (tag->numRemaining <= 0) break;
            if (id != cursor->id && has_list[cursor->id] != 1) {
                sprintf(temp, "%s{%d=%s}", temp, cursor->id, cursor->packet);
                tag->numRemaining--;
                count++;
            }
            cursor = cursor->next;
        }
        if (count == 0 )
            temp = "Empty";
    } else
        temp = "Empty";

    message = malloc(sizeof(char)*(8+strlen(temp)));
    sprintf(message, "%d|%d|%s|\0", type, count, temp); // Num packets in buffer max of Packets
    if (send(client_fd, message, strlen(message)+1, 0) == -1)
        fprintf(stdout, "sendMessage Error: sending to %d\n", client_fd);
    free(message);
}

void *listenerThread(void *args) {
    struct both *both1 = (struct both*)args;
    struct nodes *tag = both1->tag;
    int *my_socket = &both1->socket, dstID, srcID, srcX, srcY, type;
    char *buffer = malloc(sizeof(char)*BUFFSIZE);
    char *srcName = malloc(sizeof(char)*BUFFSIZE);
    char *srcHas= malloc(sizeof(char)*BUFFSIZE);

    int clientSocket = getConnections(my_socket);
    if (clientSocket == -1) {
        close(*my_socket);
        return (void*)-1;
    }

    ssize_t numbytes;
    if ((numbytes = recv(clientSocket, buffer, BUFFSIZE, 0)) == -1) {
        perror("recv");
        exit(2);
    }
    buffer[numbytes] = '\0';
    strcpy(srcName, strtok(buffer,"|"));
    type = atoi(strtok(NULL,"|"));
    srcID = atoi(strtok(NULL,"|"));
    srcX = atoi(strtok(NULL,"|"));
    srcY = atoi(strtok(NULL,"|"));
    dstID = atoi(strtok(NULL,"|"));
    strcpy(srcHas, strtok(NULL,"|"));

    if (in_range(srcX, srcY, tag)) {
        printf("%s is in range of %s and %s is closer to the base\n", srcName, tag->name, srcName);
        response(-1, clientSocket, srcID, tag, srcHas);
    } else
        response(-1, clientSocket, 0, NULL, NULL);

    free(buffer);
    free(srcName);
    free(srcHas);
    close(clientSocket);

    pthread_exit(NULL);
}

void start_simulation(struct nodes *tag_info) {
    pthread_t listen_thread;

    for (int s=0; s<NumSteps; s++) {
        for(int i=0; i<NumTags; i++){
            if (i == tag_info->id) continue; // TODO
            both1[i] = malloc(sizeof(struct both));
            both1[i]->tag = tag_info;
            both1[i]->socket = tag_info->p_sockets[i];
            printf("Listener on: %s\n", tag_info->p_ports[i]);
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

int base_in_range(int x1, int y1) {
    double x2 = pow(abs(x1 - baseStation->x), 2);
    double y2 = pow(abs(y1 - baseStation->y), 2);

    if((x2 + y2) <= pow(Range,2))
        return 1;
    return 0;

}

void base_recv(int clientSocket) {
    ssize_t numbytes;
    char *buffer = malloc(sizeof(char)*BUFFSIZE);
    printf("Base Wait\n");
    if ((numbytes = recv(clientSocket, buffer, BUFFSIZE, 0)) == -1) {
        perror("recv");
        exit(2);
    }
    buffer[numbytes] = '\0';
    printf("Base Done\n");
}

void *baseListener(void *args) {
    ssize_t numbytes;
    int *my_socket = (int*)args, srcID, srcX, srcY, type;
    char *buffer = malloc(sizeof(char)*BUFFSIZE);
    char *srcName = malloc(sizeof(char)*BUFFSIZE);

    int clientSocket = getConnections(my_socket);
    if (clientSocket == -1) {
        close(*my_socket);
        return (void*)-1;
    }

    numbytes = recv(clientSocket, buffer, BUFFSIZE, MSG_PEEK);
    if ((numbytes = recv(clientSocket, buffer, (size_t) numbytes, 0)) == -1) {
        perror("recv");
        exit(2);
    }
    buffer[numbytes] = '\0';
    strcpy(srcName, strtok(buffer,"|"));
    type = atoi(strtok(NULL,"|"));
    srcID = atoi(strtok(NULL,"|"));
    srcX = atoi(strtok(NULL,"|"));
    srcY = atoi(strtok(NULL,"|"));


    if (base_in_range(srcX, srcY)) {
        printf("In Range\n");
        response(-2, clientSocket, 0, NULL, NULL);
        base_recv(clientSocket);
    } else {
        printf("\tNot In Range\n");
        response(-1, clientSocket, 0, NULL, NULL);
    }

    free(buffer);
    free(srcName);
    close(clientSocket);
    pthread_exit(NULL);
}

void run_baseStation() {
    pthread_t baseThread;
    for (int s = 0; s < NumSteps; s++) {
        for (int i = 0; i < NumTags; i++) {
            printf("BaseListener on: %s\n", baseStation->p_ports[i]);
            pthread_create(&baseThread, NULL, baseListener, &baseStation->p_sockets[i]);
        }

        pthread_join(baseThread, NULL);
    }


//    TODO
//    start listeners
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
    Delta = Range/10;

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
        baseStation->has[i] = -1;
        baseStation->p_ports[i] = malloc(sizeof(char)*10);
        sprintf(baseStation->p_ports[i], "%d", atoi(baseStation->port)+i);
        baseStation->p_sockets[i] = initializeSocket(baseStation->p_ports[i]);
        printf("Init: B -> %d = %s_%d\n", i, baseStation->p_ports[i], baseStation->p_sockets[i]);
    }

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

        for(int j=0; j<NumTags; j++){
            if (j == i) {
                animal_tags[i]->has[j] = 1;
                animal_tags[i]->p_ports[j] = baseStation->p_ports[i];
//                animal_tags[i]->p_sockets[j] = -2;
//                printf("Init: %d -> B = %s_%d\n", i, animal_tags[i]->p_ports[j], animal_tags[i]->p_sockets[j]);
                continue;
            }
            animal_tags[i]->has[j] = -1;
            animal_tags[i]->p_ports[j] = malloc(sizeof(char)*10);
            sprintf(animal_tags[i]->p_ports[j], "%d", atoi(animal_tags[i]->port)+j);
            animal_tags[i]->p_sockets[j] = initializeSocket(animal_tags[i]->p_ports[j]);
            printf("Init: %d -> %d = %s_%d\n", i, j, animal_tags[i]->p_ports[j], animal_tags[i]->p_sockets[j]);
        }

        animal_tags[i]->numRemaining = Packets;
        struct packet_buffer *my_packet = malloc(sizeof(struct packet_buffer));
        my_packet->next = NULL;
        my_packet->id = i;
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
            if (i != j)
                free(animal_tags[i]->p_ports[j]);
        }
        free(animal_tags[i]->packets->packet);
        free(animal_tags[i]->packets);
        free(animal_tags[i]->port);
        free(animal_tags[i]->name);
        free(animal_tags[i]);
    }
    return 0;
}
