/**
 * @file clientcxn.h
 * @author KVEP - https://gitlab.com/kvep/
 * @version 1.0
 * @date 2021-12-20
 * 
 * @copyright Copyright (c) 2021 KVEP
 */

#define BUFFERSIZE 2048

void *threadProcess(void * ptr);
int open_connection();
void init_thread(int argc, char **argv);