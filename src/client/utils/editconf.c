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

#define CONF_FILE "../config.ini"

#include "editconf.h"



void main()
{
    // char *file = "../config.ini";
    // char *section = "Serveur Configuration";
    // char *field = "IP";

    // char *IP = readFiletest(file, section, field);
    // printf("IP : %s \n",IP);

    
    
    char *IP = NULL ;
    IP = malloc(50 * sizeof(char));
    readFiletest(CONF_FILE, "Serveur Configuration", "IP", &IP);
    printf("IP : %s \n",IP);

    char *port= NULL;
    port = malloc(50 * sizeof(char));
    readFiletest(CONF_FILE, "Serveur Configuration", "port", &port);
    printf("port : %s \n",port);

    char *test = NULL;
    test =  malloc(50 * sizeof(char));
    readFiletest(CONF_FILE, "Serveur Configuration", "test", &test);
    printf("test : %s \n",test);
}


/**
 * @brief return a param from a file location, a section, and a field
 * @param file 
 * @param section 
 * @param field 
 * @return char* : param in a config file .ini
 */
void readFiletest(char *file, char *section, char *field, char **param)
{
    ini_t *f = ini_load(file);
    //const char *param ;
    // printf("Size of %d\n", sizeof(param));

    if ((*param = ini_get(f, section, field)) != 0)
    {
        return;
    }
    printf("Error : the param '%s' of the configuration file is not correct \n",field);
    // perror("Configuration file is not correct"); ==> Pb SUCCESS apres le message 
    exit(-5);


}
