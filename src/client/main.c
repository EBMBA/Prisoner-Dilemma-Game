/**
 * @file main.c
 * @author KVEP - https://gitlab.com/kvep
 * @version 1.0
 * @date 2021-11-17
 * 
 * @copyright Copyright (c) 2021
 */

#include <stdio.h>
#include "views/view.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "../common/protocol/protocol.h"

#define PORT 8085
#define MAX_SOCK_SIZE 1024

int main(int argc, char **argv)
{
	init_main_window(argc,argv);
	gtk_main();
	// int sock = 0, valread;
	// struct sockaddr_in serv_addr;
	// char *hello = NULL;
	// char bufferIn[MAX_SOCK_SIZE] = {0};
	// char *bufferOut = NULL;
	// packet packetd;

	// packetd.client_id = 0;
    // packetd.game_id = 0;
    // packetd.action_id = 0;
    // packetd.result_id = 0;
    // packetd.earned_money = 0;
    // packetd.current_round = 0;
	// packetd.

	// //hello = malloc(100 * sizeof(char));

	// if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	// {
	// 	printf("\n Socket creation error \n");
	// 	return -1;
	// }

	// serv_addr.sin_family = AF_INET;
	// serv_addr.sin_port = htons(PORT);
	
	// // Convert IPv4 and IPv6 addresses from text to binary form
	// if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	// {
	// 	printf("\nInvalid address/ Address not supported \n");
	// 	return -1;
	// }

	// if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	// {
	// 	printf("\nConnection Failed \n");
	// 	return -1;
	// }

	// // Parse in bufferOut
	// bufferOut = set_parse(packetd);

	// send(sock , bufferOut , strlen(bufferOut) , 0);
	// printf("Hello message sent\n");

	// while (1)
	// {
	// 	valread = read( sock , bufferIn, MAX_SOCK_SIZE);
	// 	printf("%s\n",bufferIn);
	// }
	
	// valread = read( sock , buffer, MAX_SOCK_SIZE);
	// printf("%s\n",buffer );
	return 0;
}
