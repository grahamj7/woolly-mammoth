/* CMPT 434 - Winter 2016
 * Assignment 2, Question 1
 *
 * Jordaen Graham - jhg257
 *
 * File: sender.c
 */

#include "server.h"

struct message{
    char *text;
    int sequence_num;
};

struct message_queue{
    char *text;
    struct message_queue *next;
};

pthread_cond_t cond;
pthread_mutex_t queue_mutex, window_mutex;
struct message_queue *MessageQueue;
int sock_fd=-1, queue_size=0, window_size=WINDOW_SIZE, timeout=60, sender_count=0, sequence=0;
struct	sockaddr *ai_addr;
socklen_t ai_addrlen;
struct timeval tv_timeout;

char *readFromMessageQueue(){
    struct message_queue *temp=MessageQueue;
    char *text;
    text = MessageQueue->text;
    MessageQueue = temp;
    MessageQueue = MessageQueue->next;
    free(temp);
    queue_size--;

    return text;
}

void addToMessageQueue(char *text){
    struct message_queue *queue, *current;

    pthread_mutex_lock(&queue_mutex);
    current = MessageQueue;
    queue = (struct message_queue *) malloc(sizeof(struct message_queue));
    queue->text = malloc(strlen(text) * sizeof(char));
    snprintf(queue->text, strlen(text), "%s", text);
    queue->next=NULL;

    if (current == NULL){
        MessageQueue = queue;
    } else {
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = queue;
    }

    queue_size++;
    if (queue_size >= window_size)
        pthread_cond_signal(&cond);
    pthread_mutex_unlock(&queue_mutex);
}

void *sendMessage(void *args) {
    struct message *values = args;
    char *buffer = malloc((strlen(values->text)+2) * sizeof(char)), buf[MAXBUFLEN], *temp;
    struct sockaddr_storage their_addr; /* connector's address information */
    socklen_t addr_len;

    sprintf(buffer, "%d:%s", values->sequence_num, values->text);
    while (1) {
        if (sendto(sock_fd, buffer, strlen(buffer) + 1, 0, ai_addr, ai_addrlen) == -1) {
            perror("sender: sendto");
            exit(1);
        }

        /* Wait for ack from receiver based on value->sequence_num */
        addr_len = sizeof their_addr;
        if (recvfrom(sock_fd, buf, MAXBUFLEN - 1, 0, (struct sockaddr *) &their_addr, &addr_len) > 0) {
            temp = malloc(strlen(buf) * sizeof(char));
            strcpy(temp, buf);
            if (atoi(strtok(buf, ":")) >= values->sequence_num) {
                free(temp);
                break;
            }
            free(temp);
        }
    }

    free(values->text);
    free(values);
    free(buffer);
    sender_count--;
    pthread_exit(NULL);
}

void *sendMessagesLoop(void *_) {
    struct timeval now;
    struct timespec timeToWait;
    char *text;
    struct message *values;
    pthread_t thread;

    while (1) {
        gettimeofday(&now, NULL);
        timeToWait.tv_sec = now.tv_sec+tv_timeout.tv_sec;
        timeToWait.tv_nsec = now.tv_usec+tv_timeout.tv_usec;

        pthread_cond_timedwait(&cond, &queue_mutex, &timeToWait);
        pthread_mutex_lock(&window_mutex);
        if (sender_count <= window_size-2) {
            text = readFromMessageQueue();
            values = malloc(sizeof(struct message));
            values->text = text;
            values->sequence_num = sequence;
            sender_count++;
            sequence++;
            sequence = sequence % MAXSEQUENCE;
            if (pthread_create(&thread, NULL, sendMessage, values) == -1) {
                addToMessageQueue(text);
                sender_count--;
            }
        }
        pthread_mutex_unlock(&window_mutex);
    }
    pthread_exit(NULL);
}


void runSender(){
    char *text=NULL;
    size_t len = 0;

    while (getline(&text, &len, stdin) != -1) {
        if (strcmp(text, "\n") == 0) {
            printf("Empty Line\n");
            continue;
        }
        addToMessageQueue(text);
    }
}

int main(int argc, char *argv[]) {

    struct addrinfo hints, *server_info, *p;
    pthread_t send_thread;
    char *host = "127.0.0.1", *port = PORT;
    int rv;

    /* Get command line args */
    if (argc > 1)
        host = argv[1];
    if (argc > 2)
        port = argv[2];
    if (argc > 3)
        window_size = atoi(argv[3]);
    if (argc > 4)
        timeout = atoi(argv[4]);
    printf("Receiver: host: %s, port: %s, windows: %d, timeout: %ds\n", host, port, window_size, timeout);

    /* Connect to the Receiver */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(host, port, &hints, &server_info)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    /* loop through all the results and make a socket */
    for(p = server_info; p != NULL; p = p->ai_next) {
        if ((sock_fd = socket(p->ai_family, p->ai_socktype,
                              p->ai_protocol)) == -1) {
            perror("sender: socket");
            continue;
        }

        tv_timeout.tv_sec = timeout;
        tv_timeout.tv_usec = 0;
        if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv_timeout , sizeof(tv_timeout))  == -1) {
            perror("setsockopt: rcvtimeout");
            continue;
        }

        break;
    }

    if (p == NULL) {
        perror("sender: failed to create socket\n");
        return 2;
    }

    /* Initialize sender */
    MessageQueue = NULL;
    ai_addr = p->ai_addr; /* binary address */
    ai_addrlen = p->ai_addrlen;	/* length of ai_addr */

    if (0 != pthread_cond_init(&cond, NULL)){
        perror("init cond");
        exit(1);
    }
    if (0 != pthread_mutex_init(&queue_mutex, NULL)) {
        perror("init queue_mutex");
        exit(1);
    }
    if (0 != pthread_mutex_init(&window_mutex, NULL)) {
        perror("init window_mutex");
        exit(1);
    }



    /* Run the main program */
    /* Create and start client senders thread */
    if (pthread_create(&send_thread, NULL, sendMessagesLoop, NULL) == -1){
        perror("start pthread");
        return -1;
    }
    /* Input loop */
    runSender();


    /* Cleanup */
    if (pthread_join(send_thread, NULL) == -1){
        perror("join pthread");
        return -1;
    }

    freeaddrinfo(server_info);
    close(sock_fd);
    if (pthread_cond_destroy(&cond)) {
        perror("destroy cond");
        exit(1);
    }
    if (pthread_mutex_destroy(&queue_mutex)) {
        perror("destroy queue_mutex");
        exit(1);
    }
    if (pthread_mutex_destroy(&window_mutex)) {
        perror("destroy window_mutex");
        exit(1);
    }
    return 0;
}

