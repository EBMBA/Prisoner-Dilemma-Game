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
#include "../common/utils/editconf.h"
#include "socket/clientcxn.h"

//#define PORT 8085
#define MAX_SOCK_SIZE 1024

int main(int argc, char **argv)
{
    init_thread(argc, argv);
    
    // int sock = 0, valread;
    // struct sockaddr_in serv_addr;
    
    // char bufferIn[MAX_SOCK_SIZE] = {0};
    // char *bufferOut = NULL;

    // // Packet
    // packet *packetd;

    // packetd = init_packet(packetd, 0);
    // printf("\n Packet created \n");

    // printf("\n Packet full \n");

    // if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    // {
    //     printf("\n Socket creation error \n");
    //     return -1;
    // }

    // printf("\n Socket created \n");

    // if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    // {
    //     printf("\nConnection Failed \n");
    //     return -1;
    // }

    // // Parse in bufferOut
    // bufferOut = set_parse(*packetd);

    // send(sock, bufferOut, strlen(bufferOut), 0);
    // printf("Hello message sent\n");

    // int i = 0;

    // printf("action : ");
    // scanf("%hu", &packetd->action_id);
    // printf("\n");

    // bufferOut = set_parse(*packetd);
    // send(sock, bufferOut, strlen(bufferOut), 0);

    // while (1)
    // {
    //     valread = read(sock, bufferIn, MAX_SOCK_SIZE);
    //     printf("%s\n", bufferIn);
    //     *packetd = get_parse(bufferIn);
    //     i++;

    //     if (packetd->game_id != 0)
    //     {
    //         setID(packetd->game_id, ID_FILE);
    //     }

    //     switch (packetd->action_id)
    //     {
    //     case YOUR_TURN:
    //         printf("action : ");
    //         scanf("%hu", &packetd->action_id);
    //         printf("\n");

    //         bufferOut = set_parse(*packetd);
    //         send(sock, bufferOut, strlen(bufferOut), 0);
    //         break;

    //     case UPDATE:
    //         switch (packetd->result_id)
    //         {
    //         case WIN:
    //             printf("You win this round %u\n", packetd->earned_money);
    //             break;

    //         case LOSE:
    //             printf("You lose this round %u\n", packetd->earned_money);
    //             break;
    //         default:
    //             break;
    //         }
    //         break;
    //     case FINISH:
    //         printf("Last round\n");
    //         switch (packetd->result_id)
    //         {
    //         case WIN:
    //             printf("You win %u\n", packetd->earned_money);
    //             break;

    //         case LOSE:
    //             printf("You lose %u\n", packetd->earned_money);
    //             break;
    //         default:
    //             break;
    //         }
    //         break;
    //     case RESULTS:
    //         printf("Results time\n");
    //         switch (packetd->result_id)
    //         {
    //         case WIN:
    //             printf("You win %u\n", packetd->earned_money);

    //             // Put gameId to 0
    //             setID(0, ID_FILE);

    //             exit(0);
    //             break;

    //         case LOSE:
    //             printf("You lose %u\n", packetd->earned_money);

    //             // Put gameId to 0
    //             setID(0, ID_FILE);

    //             exit(0);
    //             break;
    //         default:
    //             break;
    //         }
    //         break;
    //     default:
    //         break;
    //     }
    // }

    exit(0);
}
