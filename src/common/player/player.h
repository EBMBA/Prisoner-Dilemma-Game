#include <stdbool.h>
#include <string.h>

#ifndef PLAYER_H
#define PLAYER_H

#define MAX_PSEUDO_LENGHT 25

typedef struct 
{
    int id;
    char pseudo[MAX_PSEUDO_LENGHT];
} Player;

Player init_player(int id, char pseudo[MAX_PSEUDO_LENGHT]);
#endif