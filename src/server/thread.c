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

    int len;
    connection_t *connection;

    if (!ptr) pthread_exit(0);
    connection = (connection_t *) ptr;
    printf("New incoming connection \n");

    add(connection);

    //Welcome the new client
    printf("Welcome #%i\n", connection->index);
    sprintf(buffer_out, "Welcome #%i\n", connection->index);
    write(connection->sockfd, buffer_out, strlen(buffer_out));

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


/*
Connection init_Configuration (){
    Connection *connexion=malloc(sizeof(Connection));
    connexion->opt = 1;
    connexion->max_clients = MAX_SIMULTANEOUS_CONNECTIONS;

    //initialise all client_socket[] to 0 so not checked
	for (int i = 0; i < connexion->max_clients; i++)
	{
		connexion->client_socket[i] = 0;
	}

    //create a master socket
	if( (connexion->master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

    //set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if( setsockopt(connexion->master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&connexion->opt,
		sizeof(connexion->opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

    //type of socket created
	connexion->address.sin_family = AF_INET;
	connexion->address.sin_addr.s_addr = INADDR_ANY;
	connexion->address.sin_port = htons( PORT );
		
	//bind the socket to localhost port 8080
	if (bind(connexion->master_socket, (struct sockaddr *)&connexion->address, sizeof(connexion->address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", PORT);


    //try to specify maximum of MAX_SIMULTANEOUS_CONNECTIONS pending connections for the master socket
	if (listen(connexion->master_socket, MAX_SIMULTANEOUS_CONNECTIONS) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

    //accept the incoming connection
	connexion->addrlen = sizeof(connexion->address);
	puts("Waiting for connections ...");

    return *connexion;
}
*/
/*   
int main()
{
    pthread_t thread_id;
    int opt = 1;
	int activity, i , valread , sd;
	int max_sd;
	struct sockaddr_in address;  

    char buffer[MAX_SOCK_SIZE]; //data buffer of 1K

    //set of socket descriptors
	fd_set readfds;

    //a message
	char *message = "ECHO Daemon v1.0 \r\n";
	
    Connection connexion = init_Configuration();

    while (1)
    {
        //clear the socket set
		FD_ZERO(&readfds);
	
		//add master socket to set
		FD_SET(connexion.master_socket, &readfds);
		max_sd = connexion.master_socket;
        puts("Max sd");

        printf("Max clients : %d \n", connexion.max_clients);

        //add child sockets to set
		for ( i = 0 ; i < connexion.max_clients ; i++)
		{
			//socket descriptor
			sd = connexion.client_socket[i];
            printf("sd descriptor ");
				
			//if valid socket descriptor then add to read list
			if(sd > 0){
                FD_SET( sd , &readfds);
                printf("valid sd");
            }
				
				
			//highest file descriptor number, need it for the select function
			if(sd > max_sd){
                max_sd = sd;
                printf("highest file descriptor number, need it for the select function");
            }
				
		}

        //wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR))
		{
			printf("select error");
		}

        //If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(connexion.master_socket, &readfds))
		{
			if ((connexion.new_socket = accept(connexion.master_socket,
					(struct sockaddr *)&address, (socklen_t*)&connexion.addrlen))<0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}
			
			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d\n ", connexion.new_socket , inet_ntoa(address.sin_addr) , ntohs
				(address.sin_port));
		
			//send new connection greeting message
			if( send(connexion.new_socket, message, strlen(message), 0) != strlen(message) )
			{
				perror("send");
			}
				
			puts("Welcome message sent successfully");
				
			//add new socket to array of sockets
			for (i = 0; i < connexion.max_clients; i++)
			{
				//if position is empty
				if( connexion.client_socket[i] == 0 )
				{
					connexion.client_socket[i] = connexion.new_socket;
					printf("Adding to list of sockets as %d\n" , i);
						
					break;
				}
			}
		}
			
		//else its some IO operation on some other socket
		for (i = 0; i < connexion.max_clients; i++)
		{
			sd = connexion.client_socket[i];
				
			if (FD_ISSET( sd , &readfds))
			{
				//Check if it was for closing , and also read the
				//incoming message
				if ((valread = read( sd , buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&connexion.addrlen);
					printf("Host disconnected , ip %s , port %d \n" ,
						inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
						
					//Close the socket and mark as 0 in list for reuse
					close( sd );
					connexion.client_socket[i] = 0;
				}
					
				//Echo back the message that came in
				else
				{
					//set the string terminating NULL byte on the end
					//of the data read
					buffer[valread] = '\0';
					send(sd , buffer , strlen(buffer) , 0 );
				}
			}
        }
    }
    
    
    //pthread_create(&thread_id, NULL, myThread, (void *)&thread_id);
  
    pthread_exit(NULL);
    return 0;
}
*/