#include <stdlib.h>
#include "game.h"

/**
 * @brief initialize the default configuration for a game
 */
int default_total_rounds = 5;
int default_round_bet = 100;

/**
 * @brief create game object
 */
Game *game;

/**
 * @brief init a game
 * @return Game* 
 */
Game *init_game()
{
    game = (Game *)malloc(sizeof(Game));
    game->total_rounds = default_total_rounds;
    game->round_bet = default_round_bet;
    game->current_round = 1;
    return game;
}

/**
 * @brief 
 * @param id 
 */
void game_set_id(unsigned int id)
{
    game->id++;
}

/**
 * @brief set the number of rounds
 * @param rounds 
 */
void game_set_total_rounds(unsigned int rounds)
{
    game->total_rounds = rounds;
}

/**
 * @brief set the amount bet for a round
 * @param sum 
 */
void game_set_round_bet(unsigned int bet)
{
    game->round_bet = bet;
}

/**
 * @brief define the player's action
 * @param action 
 */
void game_set_action(enum actions action)
{
    game->action = action;
}

/**
 * @brief check if we can go to the next round then increment it
 * @return true 
 * @return false 
 */
bool game_next_round(void)
{
    if (game->current_round + 1 <= game->total_rounds)
    {
        game->current_round++;
        return true;
    }
    return false;
}

/**
 * @brief defined the reaction time of the players
 * @param round_start_time 
 * @param action_clicked_time 
 */
void game_set_player_reacttime(struct timeval round_start_time, struct timeval action_clicked_time)
{
    game->player_reacttime = (action_clicked_time.tv_sec - round_start_time.tv_sec) * 1000.0;
    game->player_reacttime += (action_clicked_time.tv_usec - round_start_time.tv_usec) / 1000.0;
}

/**
 * @brief get the game object
 * @return Game* 
 */
Game *get_game(void)
{
    return game;
}

/**
 * @brief set the game object
 * @param game_g 
 */
void set_game(Game *game_g)
{
    game = game_g;
}