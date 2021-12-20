#include <stdbool.h>
#include "../../common/player/player.h"
#include "../../common/utils/editconf.h"
#include "../games_management/gamesM.h"

#ifndef GAME_H
#define GAME_H

typedef struct
{
    u_int16_t id;
    u_int16_t total_rounds;
    u_int16_t current_round;
    u_int16_t money_per_round;
    connection_t *player1;
    u_int16_t player1_reaction_time;
    u_int16_t player1_earned_money;
    u_int16_t player1_action_id;
    u_int16_t player1_result_id;
    connection_t *player2;
    u_int16_t player2_reaction_time;
    u_int16_t player2_earned_money;
    u_int16_t player2_action_id;
    u_int16_t player2_result_id;
} Game;

Game *init_game(Game *game);
Game *create_game(connection_t *player, Game *game);
Game *join_game(connection_t * player, Game *game);

/* void game_set_id(unsigned int id);
void game_set_total_rounds(unsigned int rounds);
void game_set_round_bet(unsigned int bet);
void game_set_action(enum actions action);
bool game_next_round(void);
void game_set_player_reacttime(struct timeval round_start_time, struct timeval action_clicked_time); */
/* Game *get_game(void);
void set_game(Game *);
void set_players_game(Game *game_g, Player player1, Player player2 ); */


#endif