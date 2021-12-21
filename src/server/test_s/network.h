#if !defined(NETWORK_H)
#define NETWORK_H

#include <sys/socket.h>

// Define a listen port 
#define PORT 8085
#define MAX_SOCK_SIZE 1025
#define MAX_SIMULTANEOUS_CONNECTIONS 100
#define BUFFER_SIZE 2048

typedef struct {
    int sockfd;
    struct sockaddr address;
    int addr_len;
    int index;
} connection_t;

void init_sockets_array();
void add(connection_t *connection);
void del(connection_t *connection);
int create_server_socket();
/**
 * Thread allowing server to handle multiple client connections
 * @param ptr connection_t 
 * @return 
 */
void *threadProcess(void *ptr);

void *myThread(void *vargp, int *master_socket);
#endif // NETWORK_H
