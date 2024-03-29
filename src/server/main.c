/**
 * @file main.c
 * @author KVEP - https://gitlab.com/kvep/
 * @version 1.0
 * @date 2021-11-17
 * 
 * @copyright Copyright (c) 2021 KVEP
 */

/*! \mainpage Homepage
 *The project realized by the KVEP team aims to model the prisoner's dilemma to be played many times by a pair of different volunteers connected on a server. \n
 *The goal is to accumulate experimental data to study the nature and specificity of the human mind. \n
 *This project is the answer to the request of the ICS (Institute of Cognitive Sciences). \n
 *\n
 *Read the README.md for more information, about the functioning and the commands in particular
 *
 */


#include "network/network.h"
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

#include "results/results.h"

#define MAX_PSEUDO_NAME 25

/**
 * @brief start the server and wait for client connection
 * @param argc nothing
 * @param argv nothing
 * @return int : code
 */
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

    printf("Result csv init\n");
    init_file_results();

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
