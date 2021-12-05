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
    u_int16_t action_id; // play, wait or betray, coop
    u_int16_t result_id; // Win or lose
    u_int16_t money;
    u_int16_t numberRound;
} packet ;

packet get_parse(char *bufferIn);
char *set_parse(packet packetd);



#endif // PROTOCOL_H


