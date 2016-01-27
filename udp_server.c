/* CMPT 434 - Winter 2016
 * Assignment 1, Question 3
 *
 * Jordaen Graham - jhg257
 *
 * File: udp_server.c
 */


#include "server.h"

struct nodes *head;
int server_fd;
struct sockaddr_storage their_addr;
socklen_t addr_len;


char *usage = "\nUsage:\n\tadd key value : add (key, value) pair, if no existing pair with same key value\n"
        "\tgetvalue key : return value from matching (key, value) pair, if any\n"
        "\tgetall : return all (key, value) pairs\n"
        "\tremove key : remove matching (key, value) pair, if any\n"
        "\tquit : exit the server\n\n";

char *add_value(char *key, char *value){
    struct nodes *node = malloc(sizeof(struct nodes));
    struct tuples *tuple = malloc(sizeof(struct tuples));
    if (key == NULL){
        return "key and value are required for add\n";
    }
    if (value == NULL) {
        strcat(key, " has failed to be added, value is required\n");
        return key;
    }
    // Create Node/Tuple
    tuple->key = malloc(10*sizeof(char));
    tuple->value = malloc(200*sizeof(char));
    strcpy(tuple->key, key);
    strcpy(tuple->value, value);
    node->next = NULL;
    node->tuple = tuple;

    // Add Node to List
    if (NULL != head)
        node->next = head;
    head = node;

    size_t size = strlen(key)+31;
    char *message = malloc(size * sizeof(char));
    snprintf(message, size, "%s: has been added successfully\n", key);
    return message;
}

char *get_value(char *key) {
    struct nodes *cursor = head;
    while (NULL != cursor) {
        if (strcmp(cursor->tuple->key, key) == 0)
            break;
        cursor = cursor->next;
    }
    size_t size = 213;
    char *message = malloc(size * sizeof(char));
    if (NULL == cursor) {
        snprintf(message, size, "key(%s) was not found in the list\n", key);
        return message;
    }
    snprintf(message, size, "%s: %s\n", cursor->tuple->key, cursor->tuple->value);
    return message;
}

char *getall_values(){
    char *result = "", *temp;
    struct nodes *cursor = head;
    size_t size;

    while (cursor != NULL){
        size = strlen(result)+strlen(cursor->tuple->key)+strlen(cursor->tuple->value)+4;
        temp = malloc(size * sizeof(char));
        snprintf(temp, size, "%s\n%s: %s", result, cursor->tuple->key, cursor->tuple->value);
        cursor = cursor->next;
        result = temp;

    }
    if (strcmp(result, "") == 0) {
        result = "There are no values in the list";
    }
    size = strlen(result)+4;
    temp = malloc(size * sizeof(char));
    snprintf(temp, size, "%s\n", result);
    return temp;
}

char *remove_value(char *key){
    struct nodes *cursor = head, *prev=NULL;
    size_t size;
    char *message;

    while (NULL != cursor){
        if (strcmp(cursor->tuple->key, key) == 0)
            break;
        prev = cursor;
        cursor = cursor->next;
    }
    if (NULL == cursor) {
        size = strlen(key)+22;
        message = malloc(size * sizeof(char));
        snprintf(message, size, "Unable to find key: %s\n", key);
        return message;
    }

    if (NULL == prev)
        head = cursor->next;
    else
        prev->next = cursor->next;

    free(cursor->tuple);
    free(cursor);
    size = strlen(key)+19;
    message = malloc(size * sizeof(char));
    snprintf(message, size, "key(%s) was removed\n", key);
    return message;
}

void send_all_values(char *value_list){
    char *value;

    value = strtok(value_list, "\n");
    while (value != NULL) {
        printf("Val: {%s}\n", value);
        if(sendto(server_fd, value, (size_t) strlen(value), 0, (struct sockaddr *)&their_addr, addr_len) == -1){ //
            perror("listener: sendto");
            exit(1);
        }
        value = strtok(NULL, "\n");
    }
}

char *evaluate(char *buffer){
    char *arg, *temp, *message;
    if (strcmp(buffer, "") == 0) {
        size_t size = strlen(usage) + 16;
        message = malloc(size* sizeof(char));
        snprintf(message, size, "Incorrect Call\n%s\n", usage);
        return message;
    }
    arg = malloc(10 * sizeof(char));
    strcpy(arg, strtok(buffer, " "));
    temp = arg;
    for ( ; *arg; ++arg) *arg = (char) tolower(*arg);
    arg = temp;

    if (strcmp(arg, "add") == 0) {
        char *key = strtok(NULL, " "), *value = strtok(NULL, " ");
        message = add_value(key, value);
    }
    else if (strcmp(arg, "getvalue") == 0) {
        message = get_value(strtok(NULL, " "));
    }
    else if (strcmp(arg, "getall") == 0) {
        message = getall_values();
        send_all_values(message);
        size_t size = 5;
        message = malloc(size* sizeof(char));
        snprintf(message, size, "DONE");
    }
    else if (strcmp(arg, "remove") == 0) {
        message = remove_value(strtok(NULL, " "));
    }
    else {
        size_t size = strlen(usage) + 16;
        message = malloc(size* sizeof(char));
        snprintf(message, size, "Incorrect Call\n%s\n", usage);
    }

    return message;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    head = NULL;

    int sockfd = -1, numbytes;
    struct addrinfo hints, *servinfo, *p;
    char buf[MAXDATASIZE];
    char *message;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        perror("listener: failed to bind socket");
        return 2;
    }

    freeaddrinfo(servinfo);
    server_fd = sockfd;

    char *pos;
    while(1) {
        printf("Server listening on port: %s\n", PORT);
        addr_len = sizeof their_addr;
        int flags = 0;
        if ((numbytes = (int) recvfrom(sockfd, buf, MAXDATASIZE - 1, flags, (struct sockaddr *) &their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(1);
        }
        if ((pos=strchr(buf, '\n')) != NULL)
            *pos = '\0';

        message = evaluate(buf);
        printf("Message: {%s}\n", message);
        // Send back results
        if(sendto(sockfd, message, (size_t) strlen(message), flags, (struct sockaddr *)&their_addr, addr_len) == -1){ //
            perror("listener: sendto");
            exit(1);
        }
        free(message);
    }

    close(sockfd);

    return 0;
}
