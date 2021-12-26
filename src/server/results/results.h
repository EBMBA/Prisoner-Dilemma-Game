/**
 * @file results.h
 * @author KVEP - https://gitlab.com/kvep/
 * @version 1.0
 * @date 2021-12-26
 * 
 * @copyright Copyright (c) 2021 KVEP
 */

#if !defined(RESULTS_H)
#define RESULTS_H

#include "../game/game.h"

#define PATH_RESULTS "results/results.csv"

void init_file_results();
void write_round_results(Game *game);
void write_final_results(Game *game);

#endif // RESULTS_H
