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

#include"ini.h"

#define CONF_FILE "../config.ini"


int main(int argc, char const *argv[])
{
    ini_t *config = ini_load(CONF_FILE);
    const char *IP = ini_get(config, "Serveur Configuration", "IP");
    if (IP) {
        printf("IP: %s\n", IP);
    }
    return 0;
}

char *readFile(const char *file, const char *section, const *char id    )
//string readConf(string "config serveur", string "IP" )