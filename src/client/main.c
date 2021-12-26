/**
 * @file main.c
 * @author KVEP - https://gitlab.com/kvep
 * @version 1.0
 * @date 2021-11-17
 *
 * @copyright Copyright (c) 2021 KVEP
 */

#include <stdio.h>
#include "views/view.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

// For clock()
#include <time.h>

#include "../common/protocol/protocol.h"
#include "../common/utils/editconf.h"
#include "socket/clientcxn.h"

//#define PORT 8085
#define MAX_SOCK_SIZE 1024

int main(int argc, char **argv)
{
    init_thread(argc, argv);
    exit(0);
}
