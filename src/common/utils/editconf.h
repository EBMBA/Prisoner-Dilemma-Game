#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if !defined(EDIFCONF_H)
#define EDIFCONF_H
#include "ini.h"
#define CLIENT_CONF_FILE "../../client/configuration/config.ini"
#define SERVER_CONF_FILE "../../server/configuration/config.ini"
#define ID_FILE "../../client/configuration/config.ini/.id"

void readFile(char *file, char *section, char *field, char **param);
int digit_to_int(char d);
u_int16_t getID();
int setID(u_int16_t ID);

#endif // EDIFCONF_H