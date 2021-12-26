/**
 * @file editconf.h
 * @author KVEP - https://gitlab.com/kvep/
 * @version 1.0
 * @date 2021-12-26
 * 
 * @copyright Copyright (c) 2021 KVEP
 */

#if !defined(EDIFCONF_H)
#define EDIFCONF_H

#define CLIENT_CONF_FILE "src/client/configuration/config.ini"
#define SERVER_CONF_FILE "src/server/configuration/config.ini"
#define ID_FILE "src/client/configuration/.id"

void readFile(char *file, char *section, char *field, char **param);
int digit_to_int(char d);
u_int16_t getID(char *id_path);
int setID(u_int16_t ID, char *id_path);

#endif // EDIFCONF_H