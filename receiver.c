/* CMPT 434 - Winter 2016
 * Assignment 2, Question 1
 *
 * Jordaen Graham - jhg257
 *
 * File: receiver.c
 */
#include "server.h"

int sock_fd=-1, acks=0, r_max=254;
double packet_loss=1.0;
char **MessageBuffer;

void sigchld_handler(int s)
{
	/* waitpid() might overwrite errno, so we save and restore it: */
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}

int corruption(char *temp) {
    char *text=NULL;
    size_t len = 0;
    int val;
    printf("Received: %s, Correct? ", temp);
    fflush(stdout);
    getline(&text, &len, stdin);
    val = 'Y' == *text || 'y' == *text;
    free(text);
    return val;
}

int acknowledge(char *num, struct sockaddr_storage their_addr, socklen_t addr_len) {
    acks++;
    if (acks*packet_loss != 0.0) {
        char *response = malloc((strlen(num) + 4) * sizeof(char));
        snprintf(response, strlen(num) + 5, "%s:ACK", num);
        if (sendto(sock_fd, response, strlen(response) + 1, 0, (struct sockaddr *) &their_addr, addr_len) == -1) {
            perror("sendto");
            exit(1);
        }
        free(response);
    }
    return atoi(num);
}

int checkBuffer(int prev_seq, struct sockaddr_storage their_addr, socklen_t addr_len) {
    char *text=NULL, *temp, *header;
    int i;

    for (i=0; i < r_max; i++) {
        text = MessageBuffer[i];
        MessageBuffer[i] = NULL;
        if (text == NULL)
            continue;
        temp = malloc(strlen(text) * sizeof(char));
        strcpy(temp, text);
        header = strtok(text, ":");
        if ((prev_seq+1)%MAXSEQUENCE == atoi(header)) {
            if (corruption(temp)) {
                prev_seq = acknowledge(header, their_addr, addr_len);
                free(temp);
            }
        } else
            MessageBuffer[i] = temp;
        free(text);
    }
    return prev_seq;
}

void runReceiver() {
    char buf[MAXBUFLEN], *pos, *temp, *header;
    int prev_num = -1;
    struct sockaddr_storage their_addr; /* connector's address information */
    socklen_t addr_len;

    while (1) {
        addr_len = sizeof their_addr;
        if (recvfrom(sock_fd, buf, MAXBUFLEN-1, 0, (struct sockaddr *) &their_addr, &addr_len) == -1) {
            perror("recvfrom");
            exit(1);
        }
        if ((pos = strchr(buf, '\n')) != NULL)
            *pos = '\0';

        temp = malloc(strlen(buf) * sizeof(char));
        strcpy(temp, buf);
        header = strtok(buf, ":");
        if ((prev_num+1)%MAXSEQUENCE == atoi(header)) {
            if (corruption(temp)) {
                prev_num = acknowledge(header, their_addr, addr_len);
                prev_num = checkBuffer(prev_num, their_addr, addr_len);
            }
            free(temp);
        }
        else {
            if (prev_num >= atoi(header)) {
                acknowledge(header, their_addr, addr_len);
                continue;
            }
            MessageBuffer[atoi(header)%r_max] = temp;
        }
    }
}

int main(int argc, char **argv)
{
	int rv, yes=1;
	struct addrinfo hints, *servinfo, *p;
    struct sigaction sa;

    if (argc > 1)
        packet_loss = atoi(argv[1]);
        if (packet_loss > 1.0)
            packet_loss = 1/packet_loss;
    if (argc > 2)
        r_max = atoi(argv[2]);

    MessageBuffer = malloc(MAXBUFLEN*r_max* sizeof(char));
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; /* set to AF_INET to force IPv4 */
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; /* use my IP */

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
	}

	/* loop through all the results and bind to the first we can */
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sock_fd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

        if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            continue;
        }

		if (bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sock_fd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); /* all done with this structure */

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(2);
    }

    sa.sa_handler = sigchld_handler; /* reap all dead processes */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    printf("server: waiting for connections...\n");

    runReceiver();

	close(sock_fd);

	return 0;
}
