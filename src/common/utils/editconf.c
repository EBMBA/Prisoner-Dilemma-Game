/**
 * @file editconf.c
 * @author KVEP - https://gitlab.com/kvep/
 * @version 1.0
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ini.h"
#include "editconf.h"

#define CLIENT_CONF_FILE "../../client/configuration/config.ini/config.ini"
#define SERVER_CONF_FILE "../../server/configuration/config.ini/config.ini"
#define ID_FILE "../../client/configuration/config.ini/.id"


void main()
{
    // u_int16_t id = getID();
    // printf("ID = %d\n",id);

    // int verif=setID(16);
    // printf("error code = %d\n",verif);

    // id = getID();
    // printf("ID = %d\n",id);

    // // char *file = "../config.ini";
    // char *section = "Serveur Configuration";
    // //char *field = "IP";
    
    // char *IP = NULL ;
    // IP = malloc(50 * sizeof(char));
    // readFile(CLIENT_CONF_FILE, section, "IP", &IP);
    // printf("IP : %s \n",IP);

    // char *port= NULL;
    // port = malloc(50 * sizeof(char));
    // readFile(CLIENT_CONF_FILE, section, "port", &port);
    // printf("port : %s \n",port);

    // char *test = NULL;
    // test =  malloc(50 * sizeof(char));
    // readFile(CLIENT_CONF_FILE, section, "test", &test);
    // printf("test : %s \n",test);
}

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
    errno=61;
    perror("Configuration file is not correct");
    exit(-5);
}


/**
 * @brief get ID from ID_FILE
 * @return u_int16_t : ID 
 */
u_int16_t getID()
{
    u_int16_t id;
    char text[128];
    char tmpc[2];

    FILE * fp;
    fp = fopen(ID_FILE,"r");
    if (fp == NULL) 
    {
        errno=2;
        perror("file '.id' is missing");
        exit(-5);
    }

    else
    {
        char c = fgetc(fp);
        text[0]=c;
        tmpc[1]=0;

        while ((c != EOF)){
            c = fgetc(fp);
            tmpc[0]=c;
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
int setID(u_int16_t ID)
{
    int ret = 0;
    FILE * fp;
    fp = fopen(ID_FILE,"w");
    if (fp == NULL) 
    {
        errno=2;
        perror("file '.id' is missing");
        exit(-5);
    }
    else
    {
        fprintf(fp, "%d",ID);
        fclose(fp);

        u_int16_t verif = getID();
        if (ID != verif)
        {
            ret = -1;
        }
    }
    return ret;
}