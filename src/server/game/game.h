#ifndef GAME_H
#define GAME_H

#include "../network/network.h"
#include "../../common/protocol/protocol.h"

#define P1_TURN 100
#define P2_TURN 101

typedef struct
{
    u_int16_t id;
    u_int16_t total_rounds;
    u_int16_t current_round;
    u_int16_t money_per_round;
    connection_t* player1;
    u_int16_t player1_reaction_time;
    u_int16_t player1_earned_money;
    u_int16_t player1_action_id;
    u_int16_t player1_result_id;
    connection_t* player2;
    u_int16_t player2_reaction_time;
    u_int16_t player2_earned_money;
    u_int16_t player2_action_id;
    u_int16_t player2_result_id;
} Game;

Game *init_game(Game *game);
Game *create_game(connection_t *player, Game *game);
Game *join_game(connection_t * player, Game *game);
void send_packet(Game *game, int signal);
Game *init_start_game(Game *game);
Game *update_game(Game *game, packet packetd);
Game *calculate_result(Game *game);

#endif