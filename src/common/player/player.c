#include "player.h"

Player init_player(int id, char pseudo[MAX_PSEUDO_LENGHT]){
    Player player;

    player.id = id;
    strcpy(player.pseudo, pseudo);

    return player;
}