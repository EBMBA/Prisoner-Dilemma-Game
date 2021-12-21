#if !defined(PROTOCOL_H)
#define PROTOCOL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define WAIT 0
#define PLAY 1
#define COOP 2
#define BETRAY 3
#define NO_RESPONSE 4
#define FINISH 5
#define START 6
#define NOT_YOUR_TURN 7
#define YOUR_TURN 8


typedef struct 
{
    u_int16_t client_id; 
    u_int16_t game_id;
    u_int16_t action_id; // wait == 0 ; play == 1 ; coop == 2 ; betray == 3 ; no reponse == 4 ; finish == 5;
    u_int16_t result_id; // wait == 0 ; win == 1; lose == 2;
    u_int16_t earned_money;
    u_int16_t current_round;
    u_int16_t time;
} packet ;

packet get_parse(char *bufferIn);
char *set_parse(packet packetd);



#endif // PROTOCOL_H


