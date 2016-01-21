/* CMPT 434 - Winter 2016
 * Assignment 1,A
 *
 * Jordaen Graham - jhg257
 *
 * File: server.c
 */

#include "server.h"


struct nodes *head;

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

void *server(void *args) {
    char *buffer, *arg, *pos, *temp, *message;
    int client_fd;
    client_fd = *(int*) args;

    while(1) {
        message = NULL;
        buffer = recvMessage(client_fd);
        if ((pos=strchr(buffer, '\n')) != NULL)
            *pos = '\0';
        if (strcmp(buffer, "quit") == 0) {
            free(buffer);
            break;
        }
        else if (strcmp(buffer, "abort") == 0) {
            break;
        }

        arg = malloc(10 * sizeof(char));
        strcpy(arg, strtok(buffer, " "));
        temp = arg;
        for ( ; *arg; ++arg) *arg = tolower(*arg);
        arg = temp;


        if (strcmp(arg, "add") == 0) {
			char *key = strtok(NULL, " ");
			char *value = strtok(NULL, " ");
            message = add_value(key, value);
        }
        else if (strcmp(arg, "getvalue") == 0) {
            message = get_value(strtok(NULL, " "));
        }
        else if (strcmp(arg, "getall") == 0) {
            message = getall_values();
        }
        else if (strcmp(arg, "remove") == 0) {
            message = remove_value(strtok(NULL, " "));
        }
        else {
            size_t size = strlen(usage) + 16;
            message = malloc(size* sizeof(char));
            snprintf(message, size, "Incorrect Call\n%s\n", usage);
        }

        sendMessage(client_fd, message);

        if (NULL != arg)
            free(arg);
        if (NULL != buffer)
            free(buffer);
    }

    close(client_fd);
    pthread_exit(NULL);
}

void run_main_server(int server_fd) {
    pthread_t client_thread;
    /* Pointer to the location of all client thread */
    int client_fd;

    /* allocate space for the client_thread */
    client_thread = (pthread_t) malloc(sizeof(pthread_t));
    if (client_thread == 0){
        perror("create thread");
        pthread_exit(NULL);
    }

    while(1) {  /* main accept() loop */
        client_fd = get_connections(server_fd);

        if (client_fd == -1) {
            continue;
        }

        if (pthread_create(&client_thread, NULL, server, &client_fd) == -1){
            perror("start pthread");
            pthread_exit(NULL);
        }
    }
}

int main(void) {

    head = NULL;

    int client_fd = start(PORT);
    run_main_server(client_fd);

    return 0;
}
