#include <stdbool.h>
#include <stdio.h>


#if !defined(GAMESM_H)
#define GAMESM_H

#include "../game/game.h"
#include "../../common/protocol/protocol.h"
#include <stdbool.h>

typedef struct 
{
    Game games[100];
    int index;
}GamesM;



void init_games_management();
void add_game(Game game);
Game *find_game(u_int16_t game_id);



#endif // GAMESM_H
