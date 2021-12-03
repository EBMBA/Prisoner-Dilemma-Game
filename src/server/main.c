#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <errno.h>
#include <string.h>
#include <stdbool.h>

// Let us create a global variable to change it in threads
int g = 0;

// Define a listen port 
#define PORT 8080
#define MAX_SOCK_SIZE 1025
#define MAX_SIMULTANEOUS_CONNECTIONS 100
#define BUFFER_SIZE 2048
#define MAX_PSEUDO_NAME 25

typedef struct  {
    int opt ;
    int master_socket;
    int addrlen;
    int new_socket;
    int client_socket[MAX_SIMULTANEOUS_CONNECTIONS];
    int max_clients;
    char buffer[MAX_SOCK_SIZE];
    struct sockaddr_in address; 
}Connection;

typedef struct {
    int sockfd;
    struct sockaddr address;
    int addr_len;
    int index;
} connection_t;

typedef struct {
    int id ;
    char pseudo[MAX_PSEUDO_NAME] ;
    int id_Game;
}Client;

connection_t* connections[MAX_SIMULTANEOUS_CONNECTIONS];


// The function to be executed by all threads
void *myThread(void *vargp, int *master_socket)
{
    // Store the value argument passed to this thread
    int *myid = (int *)vargp;
  
    // Let us create a static variable to observe its changes
    static int s = 0;
  
    // Print the argument, static and global variables
    printf("Thread ID: %d, Static: %d, Global: %d\n", *myid, ++s, ++g);
}

void init_sockets_array() {
    for (int i = 0; i < MAX_SIMULTANEOUS_CONNECTIONS; i++) {
        connections[i] = NULL;
    }
}


void add(connection_t *connection) {
    for (int i = 0; i < MAX_SIMULTANEOUS_CONNECTIONS; i++) {
        if (connections[i] == NULL) {
            connections[i] = connection;
            return;
        }
    }
    perror("Too much simultaneous connections");
    exit(-5);
}


void del(connection_t *connection) {
    for (int i = 0; i < MAX_SIMULTANEOUS_CONNECTIONS; i++) {
        if (connections[i] == connection) {
            connections[i] = NULL;
            return;
        }
    }
    perror("Connection not in pool ");
    exit(-5);
}


/**
 * Thread allowing server to handle multiple client connections
 * @param ptr connection_t 
 * @return 
 */
void *threadProcess(void *ptr) {
    char buffer_in[BUFFER_SIZE];
    char buffer_out[BUFFER_SIZE];

    // Valeur temp pour read 
    int valread; 

    int len;
    connection_t *connection;

    if (!ptr) pthread_exit(0);
    connection = (connection_t *) ptr;
    printf("New incoming connection \n");

    add(connection);

    //Welcome the new client
    // printf("Welcome #%i\n", connection->index);
    // sprintf(buffer_out, "Welcome #%i\n", connection->index);
    // write(connection->sockfd, buffer_out, strlen(buffer_out));

    valread = read(connection->sockfd, buffer_in, BUFFER_SIZE);

    if ( buffer_in == "0" )
    {
        sprintf(buffer_out, "%i", connection->index);
        write(connection->sockfd, buffer_out, strlen(buffer_out));
    }
    

    while ((len = read(connection->sockfd, buffer_in, BUFFER_SIZE)) > 0) {

        if (strncmp(buffer_in, "bye", 3) == 0) {
            break;
        }

        strcpy(buffer_out, "\nServer Echo : ");
        strncat(buffer_out, buffer_in, len);

        if (buffer_in[0] == '@') {
            for (int i = 0; i < MAX_SIMULTANEOUS_CONNECTIONS; i++) {
                if (connections[i] != NULL) {
                    write(connections[i]->sockfd, buffer_out, strlen(buffer_out));
                }
            }
        } else if (buffer_in[0] == '#') {
            int client = 0;
            int read = sscanf(buffer_in, "%*[^0123456789]%d ", &client);
            for (int i = 0; i < MAX_SIMULTANEOUS_CONNECTIONS; i++) {
                if (client == connections[i]->index) {
                    write(connections[i]->sockfd, buffer_out, strlen(buffer_out));
                    break;
                } //no client found ? : we dont care !!
            }
        } else {
            write(connection->sockfd, buffer_out, strlen(buffer_out));
        }

        //clear input buffer
        memset(buffer_in, '\0', BUFFER_SIZE);
    }
    printf("Connection to client %i ended \n", connection->index);
    close(connection->sockfd);
    del(connection);
    free(connection);
    pthread_exit(0);

}

int create_server_socket() {
    int sockfd = -1;
    struct sockaddr_in address;
    int port = 8080;

    /* create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd <= 0) {
        fprintf(stderr, "%s: error: cannot create socket\n");
        return -3;
    }


    /* bind socket to port */
    address.sin_family = AF_INET;
    //bind to all ip : 
    //address.sin_addr.s_addr = INADDR_ANY;
    //ou 0.0.0.0 
    //Sinon  127.0.0.1
    address.sin_addr.s_addr = inet_addr("0.0.0.0");
    address.sin_port = htons(port);

    /* prevent the 60 secs timeout */
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*) &reuse, sizeof (reuse));

    /* bind */
    if (bind(sockfd, (struct sockaddr *) &address, sizeof (struct sockaddr_in)) < 0) {
        fprintf(stderr, "error: cannot bind socket to port %d\n", port);
        return -4;
    }

    return sockfd;
}


int main(int argc, char** argv) {
    int sockfd = -1;
    int index = 1;
    connection_t *connection;
    pthread_t thread;


    /* init array*/
    init_sockets_array();
    /* create socket */
    sockfd = create_server_socket();

    /* listen on port , stack size 50 for incoming connections*/
    if (listen(sockfd, 50) < 0) {
        fprintf(stderr, "%s: error: cannot listen on port\n", argv[0]);
        return -5;
    }

    printf("ready and listening\n");

    //Wait for connection
    while (true) {
        /* accept incoming connections */
        connection = (connection_t *) malloc(sizeof (connection_t));
        connection->sockfd = accept(sockfd, &connection->address, &connection->addr_len);
        connection->index = index++;
        if (connection->sockfd <= 0) {
            free(connection);
        } else {
            /* start a new thread but do not wait for it */
            pthread_create(&thread, 0, threadProcess, (void *) connection);
            pthread_detach(thread);
        }
    }
    return (EXIT_SUCCESS);
}