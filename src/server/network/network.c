/**
 * @file network.c
 * @author KVEP - https://gitlab.com/kvep/
 * @version 1.0
 * @date 2021-12-03
 * 
 * @copyright Copyright (c) 2021 KVEP
 */

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

#include "network.h"
#include "../../common/protocol/protocol.h"
#include "../game/game.h"
#include "../games_management/gamesM.h"


connection_t* connections[MAX_SIMULTANEOUS_CONNECTIONS];

/**
 * @brief initializes array "connections" containing client connection
 */
void init_sockets_array() {
    for (int i = 0; i < MAX_SIMULTANEOUS_CONNECTIONS; i++) {
        connections[i] = NULL;
    }
}

/**
 * @brief adds connection in the array
 * @param connection connection to add in the array
 */
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

/**
 * @brief removes connection from the array
 * @param connection connection to delete 
 */
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
 * @brief Creates a server socket object
 * @return int : listen socket 
 */
int create_server_socket() {
    int sockfd = -1;
    struct sockaddr_in address;
    int port = PORT;

    /* create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd <= 0) {
        fprintf(stderr, "%s: error: cannot create socket\n");
        return -3;
    }


    /* bind socket to port */
    address.sin_family = AF_INET;
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


/**
 * @brief thread allowing server to handle multiple client connections
 * @param ptr connection structure
 * @return void* : nothing
 */
void *threadProcess(void *ptr) {
    char buffer_in[BUFFER_SIZE];
    char *buffer_out;
    packet packetd;

    // Valeur temp pour read 
    int valread; 

    int len;
    connection_t *connection;

    if (!ptr) pthread_exit(0);
    connection = (connection_t *) ptr;
    printf("New incoming connection \n");

    add(connection);

    valread = read(connection->sockfd, buffer_in, BUFFER_SIZE);

    packetd = get_parse(buffer_in);
    if ( packetd.client_id == 0 )
    {
        //player = init_player((u_int16_t) connection->index, connection);
        packetd.client_id = connection->index;
        buffer_out = set_parse(packetd); // sprintf(buffer_out, "%i", connection->index);
        write(connection->sockfd, buffer_out, strlen(buffer_out));
    }
    
    Game *game;
    
    if (packetd.game_id != 0)
    {
        game = update_player(packetd.game_id, connection);            
    }
    

    while ((len = read(connection->sockfd, buffer_in, BUFFER_SIZE)) > 0) {

        packetd = get_parse(buffer_in);

        switch (packetd.action_id)
        {
        case PLAY: // First time or restart after error
            // TODO check if player was not already in a game 
            if ( ( game = find_game(packetd.game_id) ) == NULL ) // Create game because no game avalaible
            {
                game = create_game(connection, game);

            }
            else{
                game = join_game(connection, game);

                // send message to player wait or play
                game = init_start_game(game);

                send_packet(game, START);
            }
            break;
        
        default:
            game = find_game(packetd.game_id);
            game = update_game(game,packetd);

            break;
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