#include "network.h"

connection_t* connections[MAX_SIMULTANEOUS_CONNECTIONS];

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


// Let us create a global variable to change it in threads
int g = 0;

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


/**
 * Thread allowing server to handle multiple client connections
 * @param ptr connection_t 
 * @return 
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

    //Welcome the new client
    // printf("Welcome #%i\n", connection->index);
    // sprintf(buffer_out, "Welcome #%i\n", connection->index);
    // write(connection->sockfd, buffer_out, strlen(buffer_out));

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

    while ((len = read(connection->sockfd, buffer_in, BUFFER_SIZE)) > 0) {

        packet packetd = get_parse(buffer_in);

        

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
            }
            
            // TODO if he wasn't : check if there are a available game or create a new game 
            break;
        
        default:
            break;
        }
        
/* 
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
        } */

        //clear input buffer
        memset(buffer_in, '\0', BUFFER_SIZE);
    }
    printf("Connection to client %i ended \n", connection->index);
    close(connection->sockfd);
    del(connection);
    free(connection);
    pthread_exit(0);

}