#include <stdbool.h>

#ifndef GAME_H
#define GAME_H

enum actions
{
    COOPERATE,
    BETRAY
};

typedef struct
{
    unsigned int id;
    unsigned int total_rounds;
    unsigned int round_bet;
    unsigned int current_round;
    enum actions action;
    double player_reacttime;
} Game;

Game *init_game(void);
void game_set_id(unsigned int id);
void game_set_total_rounds(unsigned int rounds);
void game_set_round_bet(unsigned int bet);
void game_set_action(enum actions action);
bool game_next_round(void);
void game_set_player_reacttime(struct timeval round_start_time, struct timeval action_clicked_time);
Game *get_game(void);
void set_game(Game *);

#endif