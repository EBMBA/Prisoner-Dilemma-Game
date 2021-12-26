/**
 * @file gamesM.c
 * @author KVEP - https://gitlab.com/kvep/
 * @version 1.0
 * @date 2021-12-03
 * 
 * @copyright Copyright (c) 2021 KVEP
 */

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "gamesM.h"
#include "../../common/protocol/protocol.h"

GamesM gamesM;

/**
 * @brief initialise the structure "gamesM"
 */
void init_games_management()
{
    gamesM.index = 0;
}

/**
 * @brief add game in a structure "gamesM"
 * @param game game to add in structure
 */
void add_game(Game game)
{
    gamesM.games[gamesM.index] = game;
    gamesM.index++;
    printf("Game added !\n");
}

/**
 * @brief remove game from the structure
 * @param game game to remove 
 * @return int : code
 */
int remove_game(Game game)
{
    for (int i = 0; i < 100; i++)
    {
        if (gamesM.games[i].id == game.id)
        {
            printf("Game finded and removed !\nGame id : %u\n", gamesM.games[i].id);
            gamesM.games[i].player1 = NULL;
            gamesM.games[i].player1_action_id = INIT;
            gamesM.games[i].player1_earned_money = 0;
            gamesM.games[i].player1_reaction_time = 0;
            gamesM.games[i].player1_result_id = 0;
            gamesM.games[i].player1_average_time = 0;
            gamesM.games[i].player1_total_earned = 0;

            gamesM.games[i].player2 = NULL;
            gamesM.games[i].player2_action_id = INIT;
            gamesM.games[i].player2_earned_money = 0;
            gamesM.games[i].player2_reaction_time = 0;
            gamesM.games[i].player2_result_id = 0;
            gamesM.games[i].player2_average_time = 0;
            gamesM.games[i].player2_total_earned = 0;
            
            return 0;
        }
    }
    printf("Game not finded !\n");
    return 1;
}

/**
 * @brief search a game in the structure 
 * @param game_id ID of the game to search
 * @return Game* : game found
 */
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
                printf("Available game finded !\nGame id : %u\n", gamesM.games[i].id);
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