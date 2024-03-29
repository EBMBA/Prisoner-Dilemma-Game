/**
 * @file view.h
 * @author KVEP - https://gitlab.com/kvep/
 * @version 1.0
 * @date 2021-11-17
 * 
 * @copyright Copyright (c) 2021 KVEP
 */

#ifndef LAUNCH_H
#define LAUNCH_H

#include <gtk/gtk.h>
#include "../../common/protocol/protocol.h"

void init_main_window(int argc, char **argv, int sockfd);
void update_view(packet packetReceived);
void on_window_main_destroy();
void on_buttonPlay_clicked(GtkButton *b);
void on_entryPseudo_changed(GtkEntry *e);
int timer_handler();
void on_btnCooperate_clicked(GtkButton *b);
#endif