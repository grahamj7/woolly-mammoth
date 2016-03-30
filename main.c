#include <math.h>
#include "barrier.h"

char grid[GridSize][GridSize];
pthread_barrier_t barrier;

void move(int dir, struct location *loc) {
//      move a distance, Dist, within the simulated region;
//          if the boundary of the region is hit, bounce off so that the total distance D is still
//        covered while remaining within the region
    switch(dir) {
        case 0:
//            N
            if (loc->x < Dist)
                loc->x = Dist - loc->x;
            else
                loc->x = loc->x - Dist;
            break;
        case 1:
//            NE
            if (loc->x < Dist)
                loc->x = Dist - loc->x;
            else
                loc->x = loc->x - Dist;
            if ((loc->y+Dist) >= GridSize)
                loc->y = ((GridSize-1) - ((Dist + loc->y) - (GridSize-1)));
            else
                loc->y = loc->y + Dist;
            break;
        case 2:
//            E
            if ((loc->y+Dist) >= GridSize)
                loc->y = ((GridSize-1) - ((Dist + loc->y) - (GridSize-1)));
            else
                loc->y = loc->y + Dist;
            break;
        case 3:
//            SE
            if ((loc->x+Dist) >= GridSize)
                loc->x = ((GridSize-1) - ((Dist + loc->x) - (GridSize-1)));
            else
                loc->x = loc->x + Dist;
            if ((loc->y+Dist) >= GridSize)
                loc->y = ((GridSize-1) - ((Dist + loc->y) - (GridSize-1)));
            else
                loc->y = loc->y + Dist;
            break;
        case 4:
//            S
            if ((loc->x+Dist) >= GridSize)
                loc->x = ((GridSize-1) - ((Dist + loc->x) - (GridSize-1)));
            else
                loc->x = loc->x + Dist;
            break;
        case 5:
//            SW
            if ((loc->x+Dist) >= GridSize)
                loc->x = ((GridSize-1) - ((Dist + loc->x) - (GridSize-1)));
            else
                loc->x = loc->x + Dist;
            if (loc->y < Dist)
                loc->y = Dist - loc->y;
            else
                loc->y = loc->y - Dist;
            break;
        case 6:
//            W
            if (loc->y < Dist)
                loc->y = Dist - loc->y;
            else
                loc->y = loc->y - Dist;
            break;
        case 7:
//            NW
            if (loc->x < Dist)
                loc->x = Dist - loc->x;
            else
                loc->x = loc->x - Dist;
            if (loc->y < Dist)
                loc->y = Dist - loc->y;
            else
                loc->y = loc->y - Dist;
            break;
        default:break;
    };
}

void broadcastMessage(char *text, struct location *loc) {
    struct messagesStack *queue, *current;

    pthread_mutex_lock(&queue_mutex);
    for (int i=0; i<NumTags; i++) {
        if (i == loc->id) continue;
        queue = (struct messagesStack *) malloc(sizeof(struct messagesStack));
        queue->message = malloc(strlen(text) * sizeof(char));
        queue->loc = loc;
        queue->id = loc->id;
        snprintf(queue->message, strlen(text), "%s", text);
        queue->next=NULL;

        current = messageStack[i];
        if (current == NULL){
            messageStack[i] = queue;
        } else {
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = queue;
        }
    }
    pthread_mutex_unlock(&queue_mutex);

    pthread_barrier_wait(&barrier);
}

struct messagesStack *readMessage(int my_id, int p_id) {
    struct messagesStack *cursor = messageStack[my_id], *prev=NULL;
    pthread_mutex_lock(&queue_mutex);
    while (cursor != NULL) {
        if (cursor->id == p_id) {
            if (prev != NULL)
                prev->next = cursor->next;
            else
                messageStack[my_id] = cursor->next;
            return cursor;
        }
        prev = cursor;
        cursor = cursor->next;
    }
    pthread_mutex_unlock(&queue_mutex);
    printf("NULL\n");
    return NULL;
}

void send_packets (int p_id) {
    printf("In Range\n");
//    send message to p saying # packets that will be sent
    if (p_id == -1) {
//        base station
//        send all buffered packets
    } else {
//        another animal
//        possibly send p one or more data packets; policy dependent, but must
//        obey the constraint that to conserve power each node can send to
//        other nodes only P data packets in total (over all K time steps, and
//        including those transmissions made to the base station)
//        receive message from p giving the number of data packets (could be
//        zero) that will be received from p, and then receive that number of
//        data packets, adding them to the buffer
    }
}

void in_range(struct location *loc, struct location *p_loc) {
    double r;
    r = sqrt(pow(abs(loc->x - p_loc->x), 2) + pow(abs(loc->y - p_loc->y), 2));
    if( r <= Range)
        send_packets(p_loc->id);
}

void *tag_simulator(void *args){
    struct location *loc = (struct location*) args;
    struct messagesStack *message;
    char *data;
    int dir;
    // generate data
    for (int k = 0; k < NumSteps; k++) {
        // pick a random direction
        dir = rand()%8;
        move(dir, loc);

        data = malloc(sizeof(char)*17);
        snprintf(data, 17, "Sim: %d, %d:%d\n", loc->id, loc->x, loc->y);
        broadcastMessage(data, loc);

        for (int i=0; i < NumTags; i++) {
            if (i == loc->id)
                continue;
//          receive message from p (will include at least its position information)
            message = readMessage(loc->id, i);
            printf("%d: Data_%d: %p, %s\n\t%d:%d\n", loc->id, i, message, message->message, message->loc->x, message->loc->y);
            in_range(loc, message->loc);
            free(message->message);
            free(message);
        }
        pthread_barrier_wait(&barrier);
    }
    pthread_exit(NULL);
}

void base_station(){
    printf("base_station: %d, %d, %d\n", baseStation->id, baseStation->x, baseStation->y);
}

void destory_messageStack() {
//    for (int i=0; i<NumTags; i++) {
//
//        free(messageStack[i]);
//    }
}

int main(int argc, char **argv){
    pthread_t tags[NumTags];
    srand(NULL);
    pthread_barrier_init(&barrier, NULL, NumTags);

    baseStation = malloc(sizeof(struct location));
    baseStation->id = -1;
    baseStation->x = (GridSize-1)/2;
    baseStation->y = (GridSize-1)/2;

    for(int i=0; i<GridSize; i++)
        for(int j=0; j<GridSize; j++)
            grid[i][j] = '.';
    grid[baseStation->x][baseStation->y] = 'B';

    for (int i = 0; i < NumTags; i++) {
        animals[i] = malloc(sizeof(struct location));
        animals[i]->id = i;
        animals[i]->x = rand() % GridSize;
        animals[i]->y = rand() % GridSize;
        grid[animals[i]->x][animals[i]->y] = 'A';

        if (pthread_create(&tags[i], NULL, tag_simulator, animals[i]) == -1) {
            perror("start pthread");
            return -1;
        }
    }

    base_station();

    for (int i = 0; i < NumTags; i++) {
        if (pthread_join(tags[i], NULL) == -1) {
            perror("join pthread");
            return -1;
        }
    }

    printf("Free");
    pthread_barrier_destroy(&barrier);
    free(baseStation);
    destory_messageStack();
    return 0;
}

