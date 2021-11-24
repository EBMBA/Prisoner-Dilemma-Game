#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

// Posix OS interface
#include <unistd.h>

// Define a listen port 
#define PORT 8080
#define MAX_SOCK_SIZE 1024
#define MAX_SIMULTANEOUS_CONNECTIONS 100

int main()
{
	int socket_file_handle, new_socket, value_read;
	struct sockaddr_in address;
	int option = 1;
	int addrlen = sizeof(address);
	char buffer[MAX_SOCK_SIZE] = {0};
	char *hello = "Hello from server";

	// Create socket file descriptor 
	if ((socket_file_handle = socket(AF_INET, SOCK_STREAM, 0) ) == 0 ){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080 
	if (setsockopt(socket_file_handle, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET; // For IPv4
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	// Forcefully attaching socket to the port 8080
	if ( bind(socket_file_handle, (struct sockaddr *)&address, sizeof(address) ) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(socket_file_handle, MAX_SIMULTANEOUS_CONNECTIONS) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	if ((new_socket = accept(socket_file_handle, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	
	value_read = read(new_socket, buffer, MAX_SOCK_SIZE);
	printf("%s\n", buffer);

	send(new_socket, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	
	return 0;
}