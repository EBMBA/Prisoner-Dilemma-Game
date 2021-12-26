#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "results.h"
#include "../../common/protocol/protocol.h"

void init_file_results(){
    FILE *results_file = NULL;

    results_file = fopen(PATH_RESULTS, "w");
    // if file is not created we initialize it with the columns titles 
    if(results_file != NULL) {
        fprintf(results_file, "game_id;total_rounds;current_round;money_per_round;player1;player1_reaction_time;player1_earned_money;player1_action;player1_result;player2;player2_reaction_time;player2_earned_money;player2_action;player2_result;\n");
        fclose(results_file);
    }
}
char *get_id_string(u_int16_t id_s){
    switch (id_s)
    {
    case COOP:
        return "COOP";
        break;
    case BETRAY:
        return "BETRAY";
        break;
    case NO_RESPONSE:
        return "NO RESPONSE";
        break;
    case WIN:
        return "WIN";
        break;
    case LOSE:
        return "LOSE";
        break;
    default:
        break;
    }
    
    return NULL;
}

void write_round_results(Game *game){
    FILE* results_file = NULL;
    // open the file and we write the round of the game in it
    results_file = fopen(PATH_RESULTS, "a");

    fprintf(results_file, "%u;%u;%u;%u;%u;%u;%u;%s;%s;%u;%u;%u;%s;%s;\n",
        game->id,
        game->total_rounds, 
        game->current_round,
        game->money_per_round,
        (u_int16_t)game->player1->index,
        (u_int16_t)game->player1_average_time/game->current_round,
        game->player1_earned_money,
        get_id_string(game->player1_action_id),
        get_id_string(game->player1_result_id),
        (u_int16_t)game->player2->index,
        (u_int16_t)game->player2_average_time/game->current_round,
        game->player2_earned_money,
        get_id_string(game->player2_action_id),
        get_id_string(game->player2_result_id));
    fclose(results_file);
}

void write_final_results(Game *game){
       FILE* results_file = NULL;
    // open the file and we write the round of the game in it
    results_file = fopen(PATH_RESULTS, "a");
/*game_id;total_rounds;current_round;money_per_round;
player1;player1_reaction_time;player1_earned_money;player1_action;player1_result;
player2;player2_reaction_time;player2_earned_money;player2_action;player2_result;*/


    fprintf(results_file, "%u;%u;%u;%u;%u;%u;%u;%s;%s;%u;%u;%u;%s;%s;\n",
        game->id,
        game->total_rounds, 
        game->current_round,
        game->money_per_round,
        (u_int16_t)game->player1->index,
        (u_int16_t)game->player1_average_time/game->current_round,
        game->player1_total_earned,
        "FINISH",
        get_id_string(game->player1_result_id),
        (u_int16_t)game->player2->index,
        (u_int16_t)game->player2_average_time/game->current_round,
        game->player2_total_earned,
        "FINISH",
        get_id_string(game->player2_result_id));
    fclose(results_file);
}