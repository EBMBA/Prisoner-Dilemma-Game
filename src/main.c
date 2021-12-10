/**
 * @file main.c
 * @author KVEP - https://gitlab.com/kvep
 * @version 1.0
 * @date 2021-11-17
 * 
 * @copyright Copyright (c) 2021
 */

#include <stdio.h>
#include "../src/launcher.h"

int main(int argc, char **argv)
{
	init_main_window(argc,argv);
	gtk_main();
	return (0);
}