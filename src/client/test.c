/**
 * @file main.c
 * @author KVEP - https://gitlab.com/kvep
 * @version 1.0
 * @date 2021-11-17
 * 
 * @copyright Copyright (c) 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "../common/protocol/protocol.h"
#include "../common/utils/editconf.h"

#define MAX_SOCK_SIZE 1024

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = NULL;
	char bufferIn[MAX_SOCK_SIZE] = {0};
	char *bufferOut = NULL;
	packet packetd;

	packetd.client_id = 0;
    packetd.game_id = 0;
    packetd.action_id = 0;
    packetd.result_id = 0;
    packetd.current_round = 0;
    packetd.earned_money = 0;
    packetd.time = 0;

	//hello = malloc(100 * sizeof(char));

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

    char *IP = NULL;
    IP = malloc(50 * sizeof(char));
    readFile(CLIENT_CONF_FILE, "Serveur Configuration", "IP", &IP);
    printf("Connection to : %s\n", IP);

    char *port = NULL;
    port = malloc(50 * sizeof(char));
    readFile(CLIENT_CONF_FILE, "Serveur Configuration", "port", &port);
    printf("On port : %s\n", port);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons( (u_int16_t) atoi(port));
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, IP, &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	// Parse in bufferOut
	bufferOut = set_parse(packetd);

	send(sock , bufferOut , strlen(bufferOut) , 0);
	printf("Hello message sent\n");

    int i = 0;
	while (1)
	{
		valread = read( sock , bufferIn, MAX_SOCK_SIZE);
		printf("%s\n",bufferIn);
        packetd = get_parse(bufferIn);
        i++;
        switch (i)
        {
        case 1:
            packetd.action_id = PLAY;
            bufferOut = set_parse(packetd);
            send(sock , bufferOut , strlen(bufferOut) , 0);
            break;
        /*case 2:
            packetd.action_id = PLAY;
            bufferOut = set_parse(packetd);
            break;*/
        default:
            break;
        }

        if (packetd.action_id == YOUR_TURN)
        {
            printf("it's your turn\n");
            packetd.action_id = BETRAY;
            bufferOut = set_parse(packetd);
            send(sock , bufferOut , strlen(bufferOut) , 0);
        }
        else if (packetd.action_id == NOT_YOUR_TURN)
        {
            printf("it isn't your turn\n");
        }
	}
	
	// valread = read( sock , buffer, MAX_SOCK_SIZE);
	// printf("%s\n",buffer );
	return 0;
}
