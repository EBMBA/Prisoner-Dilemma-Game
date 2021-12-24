#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "gamesM.h"
#include "../../common/protocol/protocol.h"

GamesM gamesM;

void init_games_management()
{
    gamesM.index = 0;
}

void add_game(Game game)
{
    gamesM.games[gamesM.index] = game;
    gamesM.index++;
    printf("Game added !\n");
}

Game *find_game(u_int16_t game_id)
{
    printf("Looking for a game\n");
    switch (game_id)
    {
    case 0:
        for (int i = 0; i < 100; i++)
        {
            if (gamesM.games[i].player2_action_id == WAIT)
            {
                printf("Available game finded ! Game id : %u\n", gamesM.games[i].id);
                return &gamesM.games[i];
            }
        }
        printf("No available game !\n");
        return NULL;
        break;

    default:
        for (int i = 0; i < 100; i++)
        {
            if (gamesM.games[i].id == game_id)
            {
                printf("Game finded !\n");
                // TODO : update connection of player
                return &gamesM.games[i];
            }
        }

        break;
    }

    return NULL;
}