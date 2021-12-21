/* #include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../server/network/network.h"

#ifndef PLAYER_H
#define PLAYER_H

#define MAX_PSEUDO_LENGHT 25

typedef struct 
{
    u_int16_t id;
    u_int16_t reaction_time;
    u_int16_t earned_money;
    u_int16_t action_id;
    u_int16_t result_id;
    connection_t connection;
} Player;

Player init_player(u_int16_t id, connection_t connection);

#endif */