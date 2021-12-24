#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "game.h"
#include "../../common/utils/editconf.h"
#include "../network/network.h"
#include "../../common/protocol/protocol.h"
#include "../games_management/gamesM.h"

Game *init_game(Game *game)
{
    game = malloc(sizeof(Game));

    // Number of rounds
    char *number_round = NULL;
    number_round = malloc(50 * sizeof(char));
    readFile(SERVER_CONF_FILE, "Game Configuration", "number_of_rounds", &number_round);
    game->total_rounds = (u_int16_t)atoi(number_round);

    // Money per round
    char *money = NULL;
    money = malloc(50 * sizeof(char));
    readFile(SERVER_CONF_FILE, "Game Configuration", "money_per_round", &money);
    game->money_per_round = (u_int16_t)atoi(money);

    game->current_round = 0;

    printf("Game initiated !\n");
    return game;
}

Game *create_game(connection_t *player, Game *game)
{
    srand(time(NULL));

    game = init_game(game);

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

Game *join_game(connection_t *player, Game *game)
{
    game->player2 = player;
    game->player2_action_id = PLAY;
    game->player2_earned_money = 0;
    game->player2_reaction_time = 0;
    game->player2_result_id = 0;

    printf("Second player joined game ID : %u !\n", game->id);

    return game;
}

/**
 * @brief Send packet of the game structure to both players or one
 *
 * @param game game structure
 * @param signal Start, finish, P1_TURN, P2_TURN  or update
 */
void send_packet(Game *game, int signal)
{
    packet packetdP1;
    packet packetdP2;

    char *buffer_outP1, *buffer_outP2;

    switch (signal)
    {
    case START:
        packetdP1.action_id = game->player1_action_id;
        packetdP1.client_id = (u_int16_t)game->player1->index;
        packetdP1.game_id = game->id;
        packetdP1.current_round = game->total_rounds;
        packetdP1.earned_money = game->money_per_round;
        packetdP1.result_id = game->player1_result_id;
        packetdP1.time = INIT;

        buffer_outP1 = set_parse(packetdP1);
        write(game->player1->sockfd, buffer_outP1, strlen(buffer_outP1));
        printf("START Packet for P1 sended\n");

        packetdP2.action_id = game->player2_action_id;
        packetdP2.client_id = (u_int16_t)game->player2->index;
        packetdP2.game_id = game->id;
        packetdP2.current_round = game->total_rounds;
        packetdP2.earned_money = game->money_per_round;
        packetdP2.result_id = game->player2_result_id;
        packetdP2.time = INIT;

        buffer_outP2 = set_parse(packetdP2);
        write(game->player2->sockfd, buffer_outP2, strlen(buffer_outP2));
        printf("START Packet for P2 sended\n");
        break;

    case UPDATE:
        packetdP1.action_id = UPDATE;
        packetdP1.client_id = (u_int16_t)game->player1->index;
        packetdP1.game_id = game->id;
        packetdP1.current_round = game->current_round;
        packetdP1.earned_money = game->player1_earned_money;
        packetdP1.result_id = game->player1_result_id;
        packetdP1.time = INIT;

        buffer_outP1 = set_parse(packetdP1);
        write(game->player1->sockfd, buffer_outP1, strlen(buffer_outP1));
        printf("UPDATE Packet for P1 sended\n");

        packetdP2.action_id = UPDATE;
        packetdP2.client_id = (u_int16_t)game->player2->index;
        packetdP2.game_id = game->id;
        packetdP2.current_round = game->current_round;
        packetdP2.earned_money = game->player2_earned_money;
        packetdP2.result_id = game->player2_result_id;
        packetdP2.time = INIT;

        buffer_outP2 = set_parse(packetdP2);
        write(game->player2->sockfd, buffer_outP2, strlen(buffer_outP2));
        printf("UPDATE Packet for P2 sended\n");
        break;

    case P2_TURN:
        packetdP1.action_id = NOT_YOUR_TURN;
        packetdP1.client_id = (u_int16_t)game->player1->index;
        packetdP1.game_id = game->id;
        packetdP1.current_round = game->current_round;
        packetdP1.earned_money = INIT;
        packetdP1.result_id = INIT;
        packetdP1.time = INIT;

        buffer_outP1 = set_parse(packetdP1);
        write(game->player1->sockfd, buffer_outP1, strlen(buffer_outP1));
        printf("NOT YOUR TURN Packet for P1 sended\n");

        packetdP2.action_id = YOUR_TURN;
        packetdP2.client_id = (u_int16_t)game->player2->index;
        packetdP2.game_id = game->id;
        packetdP2.current_round = game->current_round;
        packetdP2.earned_money = INIT;
        packetdP2.result_id = INIT;
        packetdP2.time = INIT;

        buffer_outP2 = set_parse(packetdP2);
        write(game->player2->sockfd, buffer_outP2, strlen(buffer_outP2));
        printf("YOUR TURN Packet for P2 sended\n");
        break;

    case P1_TURN:
        packetdP1.action_id = YOUR_TURN;
        packetdP1.client_id = (u_int16_t)game->player1->index;
        packetdP1.game_id = game->id;
        packetdP1.current_round = game->current_round;
        packetdP1.earned_money = INIT;
        packetdP1.result_id = INIT;
        packetdP1.time = INIT;

        buffer_outP1 = set_parse(packetdP1);
        write(game->player1->sockfd, buffer_outP1, strlen(buffer_outP1));
        printf("YOUR TURN Packet for P1 sended\n");

        packetdP2.action_id = NOT_YOUR_TURN;
        packetdP2.client_id = (u_int16_t)game->player2->index;
        packetdP2.game_id = game->id;
        packetdP2.current_round = game->current_round;
        packetdP2.earned_money = INIT;
        packetdP2.result_id = INIT;
        packetdP2.time = INIT;

        buffer_outP2 = set_parse(packetdP2);
        write(game->player2->sockfd, buffer_outP2, strlen(buffer_outP2));
        printf("NOT YOUR TURN Packet for P2 sended\n");
        break;

    default:
        break;
    }
}

Game *init_start_game(Game *game)
{
    game->player1_action_id = YOUR_TURN;
    game->player2_action_id = NOT_YOUR_TURN;

    return game;
}

Game *update_game(Game *game, packet packetd)
{
    if (game->player1->index == packetd.client_id)
    {
        game->player1_action_id = packetd.action_id;
        game->player1_reaction_time = packetd.time;

        send_packet(game, P2_TURN);
    }
    else if (game->player2->index == packetd.client_id)
    {
        game->player2_action_id = packetd.action_id;
        game->player2_reaction_time = packetd.time;

        if (game->current_round < game->total_rounds)
        {
            game->current_round++;
            game = calculate_result(game);
            send_packet(game, UPDATE);

            sleep(WAITING_RESULT_TIME);

            send_packet(game, P1_TURN);
        }
        else if (game->current_round == game->total_rounds)
        {
            game = calculate_result(game);
            // TODO calculated final result for all game
            // TODO export results on CSV
            send_packet(game, FINISH);
            // TODO remove the game of the game_management
        }
    }

    return game;
}

Game *calculate_result(Game *game)
{
    printf("Game %u caculating results...\n", game->id);
    if (game->player1_action_id == COOP && game->player2_action_id == COOP)
    {
        printf("Game %u both players decide to cooperate\n", game->id);

        // For player1
        game->player1_result_id = WIN;
        game->player1_earned_money = (u_int16_t)game->money_per_round / 2;

        // For player2
        game->player2_result_id = WIN;
        game->player2_earned_money = (u_int16_t)game->money_per_round / 2;
    }
    else if (game->player1_action_id == BETRAY && game->player2_action_id == COOP)
    {
        printf("Game %u a player decides to betray the other player who cooperates with him\n", game->id);

        // For player1
        game->player1_result_id = WIN;
        game->player1_earned_money = game->money_per_round;

        // For player2
        game->player2_result_id = LOSE;
        game->player2_earned_money = 0;
    }
    else if (game->player1_action_id == COOP && game->player2_action_id == BETRAY)
    {
        printf("Game %u a player decides to betray the other player who cooperates with him\n", game->id);

        // For player1
        game->player1_result_id = LOSE;
        game->player1_earned_money = 0;

        // For player2
        game->player2_result_id = WIN;
        game->player2_earned_money = game->money_per_round;
    }
    else if (game->player1_action_id == BETRAY && game->player2_action_id == BETRAY)
    {
        printf("Game %u both player decides to betray \n", game->id);

        // For player1
        game->player1_result_id = LOSE;
        game->player1_earned_money = 0;

        // For player2
        game->player2_result_id = LOSE;
        game->player2_earned_money = 0;
    }
    else if (game->player1_action_id == NO_RESPONSE && game->player2_action_id != NO_RESPONSE)
    {
        printf("Game %u player1 don't answer \n", game->id);

        // For player1
        game->player1_result_id = LOSE;
        game->player1_earned_money = 0;

        // For player2
        game->player2_result_id = WIN;
        game->player2_earned_money = game->money_per_round;
    }
    else if (game->player1_action_id != NO_RESPONSE && game->player2_action_id == NO_RESPONSE)
    {
        printf("Game %u player2 don't answer \n", game->id);

        // For player1
        game->player1_result_id = WIN;
        game->player1_earned_money = game->money_per_round;

        // For player2
        game->player2_result_id = LOSE;
        game->player2_earned_money = 0;
    }
    else if (game->player1_action_id == NO_RESPONSE && game->player2_action_id == NO_RESPONSE)
    {
        printf("Game %u player1 and player2 don't answer \n", game->id);

        // For player1
        game->player1_result_id = LOSE;
        game->player1_earned_money = 0;

        // For player2
        game->player2_result_id = LOSE;
        game->player2_earned_money = 0;
    }

    return game;
}