#include "protocol.h"

packet get_parse(char *bufferIn)
{
    packet packetd;
    const char separator[2] = ";";
    char *token;
    u_int16_t *tokenI = malloc(sizeof(packet)); 
    int i = 0 ;

    printf("Protocol 1 \n");
    // get the first token
    token = strtok(bufferIn, separator);
    printf("Protocol 2 \n");
    //tokenI[0] = atoi(token);
    printf("Protocol 3 \n");
    /* walk through other tokens */
    while (token != NULL)
    {
        printf(" %s\n", token);
        tokenI[i] = atoi(token);
        token = strtok(NULL, separator);
        i++;
    }
    printf("Protocol 4 \n");
    packetd.client_id = tokenI[0];
    packetd.game_id = tokenI[1];
    packetd.action_id = tokenI[2];
    packetd.result_id = tokenI[3];
    packetd.earned_money = tokenI[4];
    packetd.time = token[5];
    packetd.current_round = tokenI[6];

    return packetd;
}

char *set_parse(packet packetd)
{
    printf("Protocol 5 \n");
    char *bufferOut = malloc(sizeof(packetd));

    sprintf(bufferOut, "%u;%u;%u;%u;%u;%u;%u",
            packetd.client_id,
            packetd.game_id,
            packetd.action_id,
            packetd.result_id,
            packetd.earned_money,
            packetd.time,
            packetd.current_round);
    printf("Protocol 6 \n");
    return bufferOut;
}

/*int main(int argc, char const *argv[])
{
    packet packetd;

    packetd.client_id = 0;
    packetd.game_id = 1;
    packetd.action_id = 2;
    packetd.result_id = 3;
    packetd.money = 4;
    packetd.numberRound = 5;

    char *buffer;

    buffer = set_parse(packetd);
    packetd = get_parse(buffer);

    printf("%u;%u;%u;%u;%u;%u\n",
            packetd.client_id,
            packetd.game_id,
            packetd.action_id,
            packetd.result_id,
            packetd.money,
            packetd.numberRound);
    return 0;
}
*/
