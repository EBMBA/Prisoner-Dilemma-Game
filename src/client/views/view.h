#ifndef LAUNCH_H
#define LAUNCH_H
//#include <gtk-3.0/gtk/gtk.h>
#include <gtk/gtk.h>
//#include <gtk/gtk.h>

void init_main_window(int argc, char **argv);
void on_window_main_destroy();
void on_buttonPlay_clicked(GtkButton *b);
void on_entryPseudo_changed(GtkEntry *e);
#endif