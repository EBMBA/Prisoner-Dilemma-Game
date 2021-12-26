/**
 * @file clientcxn.c
 * @author KVEP - https://gitlab.com/kvep/
 * @version 1.0
 * @date 2021-12-20
 * 
 * @copyright Copyright (c) 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../common/protocol/protocol.h"
#include "../../common/utils/editconf.h"
#include "../views/view.h"
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "clientcxn.h"

/**
 * @brief receiving data from the server and transmit information to update_view
 * @param ptr connection socket
 * @return void* : nothing
 */
void *threadProcess(void * ptr) {
    char buffer_in[BUFFERSIZE];
    int sockfd = *((int *) ptr);
    int len;
    while ((len = read(sockfd, buffer_in, BUFFERSIZE)) != 0) {
        if (strncmp(buffer_in, "exit", 4) == 0) {
            break;
        }

        packet packetd;
        packetd = get_parse(buffer_in);
        
        update_view(packetd);
    }

    // a deplacer pourafficher la derniere fenetre des resultats
    //close(sockfd);
}

/**
 * @brief start the connection between client and server
 * @return int : connection socket
 */
int open_connection() {
    int sockfd;

    struct sockaddr_in serverAddr;
    char *port = NULL;
    port = malloc(50 * sizeof(char));
    readFile("src/client/configuration/config.ini", "Serveur Configuration", "port", &port);

    // Create the socket. 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //Configure settings of the server address
    // Address family is Internet 
    serverAddr.sin_family = AF_INET;
    //Set port number, using htons function 
    serverAddr.sin_port = htons((u_int16_t)atoi(port));
    //Set IP address to localhost
    char *IP = NULL;
    IP = malloc(50 * sizeof(char));
    readFile("src/client/configuration/config.ini", "Serveur Configuration", "IP", &IP);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, IP, &serverAddr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    serverAddr.sin_addr.s_addr = inet_addr(IP);

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    //Connect the socket to the server using the address
    if (connect(sockfd, (struct sockaddr *) &serverAddr, sizeof (serverAddr)) != 0) {
        printf("Fail to connect to server");
        exit(-1);
    };

    return sockfd;
}

/**
 * @brief initialize thread, open the connection and start the first graphic window
 * @param argc 
 * @param argv 
 */
void init_thread(int argc, char **argv){
    int sockfd;
    int status = 0;
    char msg[100];
    pthread_t thread;

    sockfd = open_connection();
    init_main_window(argc,argv, sockfd);
}