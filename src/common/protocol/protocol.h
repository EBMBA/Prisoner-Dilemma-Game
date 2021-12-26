#if !defined(PROTOCOL_H)
#define PROTOCOL_H

#define INIT 0
#define WAIT 1
#define PLAY 2
#define COOP 3
#define BETRAY 4
#define NO_RESPONSE 5
#define FINISH 6
#define START 7
#define NOT_YOUR_TURN 8
#define YOUR_TURN 9
#define UPDATE 10
#define RESULTS 11

#define WIN 1
#define LOSE 2

#define WAITING_RESULT_TIME 3
#define TIME_FOR_ROUND 5

#define CLIENT_PATH "src/client/configuration/.id"

typedef struct
{
    u_int16_t client_id;
    u_int16_t game_id;
    u_int16_t action_id; // wait == 0 ; play == 1 ; coop == 2 ; betray == 3 ; no reponse == 4 ; finish == 5;
    u_int16_t result_id; // wait == 0 ; win == 1; lose == 2;
    u_int16_t earned_money;
    u_int16_t current_round;
    u_int16_t time;         
    
} packet;

packet *init_packet(packet *packetd, int client_OR_server);
packet get_parse(char *bufferIn);
char *set_parse(packet packetd);

#endif // PROTOCOL_H
