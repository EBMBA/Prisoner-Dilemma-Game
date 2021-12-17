#if !defined(PROTOCOL_H)
#define PROTOCOL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

typedef struct 
{
    u_int16_t client_id; 
    u_int16_t game_id;
    u_int16_t action_id; // wait == 0 ; play == 1 ; coop == 2 ; betray == 3 ; no reponse == 4 ; finish == 5;
    u_int16_t result_id; // Win or lose
    u_int16_t earned_money;
    u_int16_t current_round;
    u_int16_t time;
} packet ;

packet get_parse(char *bufferIn);
char *set_parse(packet packetd);



#endif // PROTOCOL_H


