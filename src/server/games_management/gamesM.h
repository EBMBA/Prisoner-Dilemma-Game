/**
 * @file gamesM.h
 * @author KVEP - https://gitlab.com/kvep/
 * @version 1.0
 * @date 2021-12-03
 * 
 * @copyright Copyright (c) 2021 KVEP
 */

#include <stdbool.h>
#include <stdio.h>

#if !defined(GAMESM_H)
#define GAMESM_H

#include "../game/game.h"
#include "../../common/protocol/protocol.h"
#include <stdbool.h>

/**
 * @brief structure of GamesM containing all games with an index 
 */
typedef struct 
{
    Game games[100];
    int index;
}GamesM;


void init_games_management();
void add_game(Game game);
Game *find_game(u_int16_t game_id);
void update_player(u_int16_t game_id, connection_t *player);
int remove_game(Game game);


#endif // GAMESM_H
