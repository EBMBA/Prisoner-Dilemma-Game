#include <stdlib.h>
#include "game.h"



/**
 * @brief create game object
 */
//Game game;

Game *init_game(Game *game){
    game = malloc(sizeof(Game));

    // Number of rounds 
    char *number_round = NULL;
    number_round =  malloc(50 * sizeof(char));
    readFile(SERVER_CONF_FILE, "Game Configuration", "number_of_rounds", &number_round);
    game->total_rounds = (u_int16_t) atoi(number_round);

    // Money per round
    char *money = NULL;
    money =  malloc(50 * sizeof(char));
    readFile(SERVER_CONF_FILE, "Game Configuration", "money_per_round", &money);
    game->money_per_round = (u_int16_t) atoi(money);

    game->current_round = 0;

    printf("Game initiated !\n");
    return game;
}


Game *create_game(connection_t *player, Game *game){
    srand(time(NULL));

    game = init_game(&game);
    
    game->id = rand() % 65536; // 2^16 
    printf("Game ID : %u !\n", game->id);
    
    game->player1 = player;
    game->player1_action_id = PLAY;
    game->player1_earned_money = 0;
    game->player1_reaction_time = 0;
    game->player1_result_id = 0;

    game->player2 = NULL;
    game->player2_action_id = WAIT;
    game->player2_earned_money = 0;
    game->player2_reaction_time = 0;
    game->player2_result_id = 0;

    add_game(*game);

    return game;
}



Game *join_game(connection_t * player, Game *game){
    game->player2 = player;
    game->player2_action_id = PLAY;
    game->player2_earned_money = 0;
    game->player2_reaction_time = 0;
    game->player2_result_id = 0;
    
    printf("Second player joined game ID : %u !\n", game->id);

    return game;
}



//
/**
 * @brief initialize the default configuration for a game
 */
/* int default_total_rounds = 5;
int default_round_bet = 100; */



/**
 * @brief init a game
 * @return Game* 
 */
/* Game *init_game()
{
    game = (Game *)malloc(sizeof(Game));
    game->total_rounds = default_total_rounds;
    game->current_round = 1;
    return game;
}
 */
/**
 * @brief 
 * @param id 
 */
/* void game_set_id(unsigned int id)
{
    game->id++;
}
 */
/**
 * @brief set the number of rounds
 * @param rounds 
 */
/* void game_set_total_rounds(u_int16_t rounds)
{
    game->total_rounds = rounds;
}
 */
/**
 * @brief define the player's action
 * @param action 
 */
/* void game_set_player1_action(u_int16_t action)
{
    game->player1.action_id = action;
}
 */
/**
 * @brief check if we can go to the next round then increment it
 * @return true 
 * @return false 
 */
/* bool game_next_round(void)
{
    if (game->current_round + 1 <= game->total_rounds)
    {
        game->current_round++;
        return true;
    }
    return false;
}
 */
/**
 * @brief defined the reaction time of the players
 * @param round_start_time 
 * @param action_clicked_time 
 */
/* void game_set_player_reacttime(struct timeval round_start_time, struct timeval action_clicked_time)
{
    game->player_reacttime = (action_clicked_time.tv_sec - round_start_time.tv_sec) * 1000.0;
    game->player_reacttime += (action_clicked_time.tv_usec - round_start_time.tv_usec) / 1000.0;
}
 */

/**
 * @brief get the game object
 * @return Game* 
 */
/* Game *get_game(void)
{
    return game;
}
 */
/**
 * @brief set the game object
 * @param game_g 
 */
/* void set_game(Game *game_g)
{
    game = game_g;
}

void set_players_game(Game *game_g, Player player1, Player player2 ){
    game_g->player1 = player1;
    game_g->player2 = player2;
}
 */
