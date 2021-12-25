#include <stdio.h>
#include <stdlib.h>
#include "view.h"

GtkBuilder *builder = NULL;
GtkBuilder *builderJeu = NULL;
GtkWidget *window;
GtkWidget *winJeu;
char tmp[128];
char tmpBet[128];
int bet=50;
GtkWidget *labelPseudo;
GtkWidget *labelBet;

int elapsed_time = 0;
int timer_id = 0;

void init_main_window(int argc, char **argv)
{
    // const char * game_path = getenv( "PWD" );
    // char path[128];
    // strcpy(path, game_path);
    // strcat(path, "/src/client/glade/Interface.glade");
    
    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("src/client/glade/Interface.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "app_win"));
    g_signal_connect(G_OBJECT(window), "destroy",(GCallback) on_window_main_destroy, NULL); 
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(window);
    
}

int timer_handler() {
    elapsed_time++;
    char txt[100];
    printf("timer running, time : %d\n", elapsed_time);
    GtkLabel *timelabel = GTK_LABEL(gtk_builder_get_object(builder, "time_display"));
    snprintf(txt, 100, "%04i", elapsed_time);
    gtk_label_set_text(timelabel, txt);
    return 1;
}

void on_buttonPlay_clicked(GtkButton *b)
{
    // const char * game_path = getenv( "PWD" );
    // char path[128];
    // strcpy(path, game_path);
    // strcat(path, "/src/client/glade/InterfaceJeu1.glade");

    builderJeu = gtk_builder_new_from_file("src/client/glade/InterfaceJeu1.glade");
    winJeu = GTK_WIDGET(gtk_builder_get_object(builderJeu, "app_jeu"));
    gtk_widget_show(winJeu);
    g_signal_connect(G_OBJECT(winJeu), "destroy",(GCallback) on_window_main_destroy, NULL);
    //gtk_widget_hide(window);

    labelPseudo = GTK_WIDGET(gtk_builder_get_object(builderJeu, "lblPseudoDisplay"));
    gtk_label_set_text(GTK_LABEL(labelPseudo), (const gchar* ) tmp);

    labelBet = GTK_WIDGET(gtk_builder_get_object(builderJeu, "lblBet"));
    sprintf(tmpBet, "Il y'a %d€ en jeu", bet);
    gtk_label_set_text(GTK_LABEL(labelBet), (const gchar* ) tmpBet);

    if (timer_id == 0) {
        timer_id = g_timeout_add(1000, (GSourceFunc) timer_handler, NULL);
    } else {
        g_source_remove(timer_id);
        timer_id = 0;
    }
    
}

void on_btnCooperate_clicked(GtkButton *b){
}
void on_btnBetray_clicked(GtkButton *b){
}
void on_entryPseudo_changed(GtkEntry *e)
{
    sprintf(tmp,"%s, que décidez-vous de faire ?", gtk_entry_get_text(e));
    //gtk_label_set_text(GTK_LABEL(labelPseudo), (const gchar*) tmp);
}

void on_window_main_destroy()
{
    gtk_main_quit();
}