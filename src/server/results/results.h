#if !defined(RESULTS_H)
#define RESULTS_H

#include "../game/game.h"

#define PATH_RESULTS "Results/results.csv"

void init_file_results();
void write_round_results(Game *game);
void write_final_results(Game *game);

#endif // RESULTS_H
