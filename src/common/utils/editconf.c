/**
 * @file editconf.c
 * @author KVEP - https://gitlab.com/kvep/
 * @version 1.0
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021 KVEP
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "editconf.h"
#include "ini.h"


/*
void main()
{
    u_int16_t id = getID();
    printf("ID = %d\n",id);

    int verif=setID(16);
    printf("error code = %d\n",verif);

    id = getID();
    printf("ID = %d\n",id);

    char *IP = NULL;
    IP = malloc(50 * sizeof(char));
    readFile(CLIENT_CONF_FILE, "Serveur Configuration", "IP", &IP);
    printf("%s\n", IP);

    char *port = NULL;
    port = malloc(50 * sizeof(char));
    readFile(CLIENT_CONF_FILE, "Serveur Configuration", "port", &port);
    printf("%s\n", port);

    char *pseudo = NULL;
    pseudo =  malloc(50 * sizeof(char));
    readFile(CLIENT_CONF_FILE, "Player", "pseudo", &pseudo);
    printf("%s\n",pseudo);

    char *number_round = NULL;
    number_round =  malloc(50 * sizeof(char));
    readFile(SERVER_CONF_FILE, "Game Configuration", "number_of_rounds", &number_round);
    printf("%s\n",number_round);

    char *money = NULL;
    money =  malloc(50 * sizeof(char));
    readFile(SERVER_CONF_FILE, "Game Configuration", "money_per_round", &money);
    printf("%s\n",money);
}
*/


/**
 * @brief return a param from a .ini conf file
 * @param file path of file 
 * @param section section in which the param is
 * @param field field of param to return
 * @return char* : param in a config file .ini
 */
void readFile(char *file, char *section, char *field, char **param)
{
    ini_t *f = ini_load(file);

    if ((*param = ini_get(f, section, field)) != 0)
    {
        ini_free(f);
        return;
    }
    ini_free(f);
    errno = 61;
    perror("Configuration file is not correct");
    exit(-5);
}

/**
 * @brief get ID from ID_FILE
 * @return u_int16_t : ID 
 */
u_int16_t getID(char *id_path)
{
    u_int16_t id;
    char text[128];
    char tmpc[2];

    FILE *fp;
    fp = fopen(id_path, "r");
    if (fp == NULL)
    {
        errno = 2;
        perror("file '.id' is missing");
        exit(-5);
    }

    else
    {
        char c = fgetc(fp);
        text[0] = c;
        tmpc[1] = 0;

        while ((c != EOF))
        {
            c = fgetc(fp);
            tmpc[0] = c;
            strcat(text, tmpc);
        }
        id = atoi(text);
    }
    fclose(fp);
    return id;
}

/**
 * @brief set ID in ID_FILE
 * @param ID value to write in the ID_FILE
 * @return int : error code
 */
int setID(u_int16_t ID, char *id_path)
{
    int ret = 0;
    FILE *fp;
    fp = fopen(ID_FILE, "w");
    if (fp == NULL)
    {
        errno = 2;
        perror("file '.id' is missing");
        exit(-5);
    }
    else
    {
        fprintf(fp, "%d", ID);
        fclose(fp);

        u_int16_t verif = getID(id_path);
        if (ID != verif)
        {
            ret = -1;
        }
    }
    return ret;
}