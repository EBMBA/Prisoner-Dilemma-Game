/**
 * @file protocol.c
 * @author KVEP - https://gitlab.com/kvep/
 * @version 1.0
 * @date 2021-12-03
 * 
 * @copyright Copyright (c) 2021 KVEP
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "protocol.h"
#include "../utils/editconf.h"

/**
 * @brief initializes the game packet 
 * @param packetd game packet
 * @param client_OR_server identification : 0 for client, 1 for server
 * @return packet* : game packet updated
 */
packet *init_packet(packet *packetd, int client_OR_server)
{
    packetd = malloc(sizeof(packet));

    switch (client_OR_server)
    {
    case 0:
        packetd->action_id = WAIT;
        packetd->client_id = 0;
        packetd->game_id = getID(CLIENT_PATH);
        packetd->current_round = 0;
        packetd->earned_money = 0;
        packetd->result_id = 0;
        packetd->time = 0;
        break;

    default:
        break;
    }

    return packetd;
}

/**
 * @brief Gets the parse object : convert a "char" in packet
 * @param bufferIn "char" to convert in packet
 * @return packet : result of the converstion 
 */
packet get_parse(char *bufferIn)
{
    packet packetd;
    const char separator[2] = ";";
    char *token;
    u_int16_t *tokenI = malloc(sizeof(packet));
    int i = 0;

    //  get the first token
    token = strtok(bufferIn, separator);

    /* walk through other tokens */
    while (token != NULL)
    {
        printf(" %s\n", token);
        tokenI[i] = atoi(token);
        token = strtok(NULL, separator);
        i++;
    }
    // printf("Protocol 4 \n");
    packetd.client_id = tokenI[0];
    packetd.game_id = tokenI[1];
    packetd.action_id = tokenI[2];
    packetd.result_id = tokenI[3];
    packetd.earned_money = tokenI[4];
    packetd.time = tokenI[5];
    packetd.current_round = tokenI[6];

    printf("Packet parsed \n");

    return packetd;
}

/**
 * @brief Sets the parse object : convert a packet in "char"
 * @param packetd packet to convert in "char"
 * @return char* : result of the converstion  
 */
char *set_parse(packet packetd)
{
    // printf("Protocol 5 \n");
    char *bufferOut = malloc(sizeof(packetd));

    sprintf(bufferOut, "%u;%u;%u;%u;%u;%u;%u",
            packetd.client_id,
            packetd.game_id,
            packetd.action_id,
            packetd.result_id,
            packetd.earned_money,
            packetd.time,
            packetd.current_round);
    printf("Packet to buffer : ok \n");
    return bufferOut;
}
