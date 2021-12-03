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
#include"ini.h"
#include "editconf.h"

#define CONF_FILE "../configuration/config.ini"


void main()
{
    idClient();
    // // char *file = "../config.ini";
    // char *section = "Serveur Configuration";
    // //char *field = "IP";
    
    // char *IP = NULL ;
    // IP = malloc(50 * sizeof(char));
    // readFile(CONF_FILE, section, "IP", &IP);
    // printf("IP : %s \n",IP);

    // char *port= NULL;
    // port = malloc(50 * sizeof(char));
    // readFile(CONF_FILE, section, "port", &port);
    // printf("port : %s \n",port);

    // char *test = NULL;
    // test =  malloc(50 * sizeof(char));
    // readFile(CONF_FILE, section, "test", &test);
    // printf("test : %s \n",test);


}

/**
 * @brief return a param from a file location, a section, and a field
 * @param file 
 * @param section 
 * @param field 
 * @return char* : param in a config file .ini
 */
void readFile(char *file, char *section, char *field, char **param)
{
    ini_t *f = ini_load(file);
    //const char *param ;
    // printf("Size of %d\n", sizeof(param));

    if ((*param = ini_get(f, section, field)) != 0)
    {
        ini_free(f);
        return;
    }
    // printf("Error : the param '%s' of the configuration file is not correct \n",field);
    ini_free(f);
    errno=61;
    perror("Configuration file is not correct ");
    exit(-5);
}


void idClient()
{
    int id;
    char text[128];
    char tmpc[2];

    FILE * fp;
    fp = fopen("../configuration/.id","rb");

    if (fp == NULL) {
        errno=2;
        perror("file '.id' is missing ");
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
    if (id == 0)
    {
        printf("ID == 0\n");
    }
    else
    {
        printf("ID = %d\n",id);
    }

}