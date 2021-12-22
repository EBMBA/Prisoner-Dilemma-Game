#include <stdio.h>
#include <stdlib.h>
#include "view.h"

GtkWidget *window;
GtkWidget *winJeu;
char tmp[128];
char tmpBet[128];
int bet=50;
GtkWidget *labelPseudo;
GtkWidget *labelBet;

char pathEnv[128]="/home/student/Documents/prisoner_dilemma/";

void init_main_window(int argc, char **argv)
{
    char path[128];
    strcpy(path, pathEnv);
    strcat(path, "src/client/glade/Interface.glade");

    gtk_init(&argc, &argv);
    GtkBuilder *builder = gtk_builder_new_from_file(path);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "app_win"));
    g_signal_connect(G_OBJECT(window), "destroy",(GCallback) on_window_main_destroy, NULL); 
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(window);
    
    GtkWidget *buttonPlay;
    buttonPlay = GTK_WIDGET(gtk_builder_get_object(builder, "buttonPlay"));
}

void on_buttonPlay_clicked(GtkButton *b){

    char path[128];
    strcpy(path, pathEnv);
    strcat(path, "src/client/glade/InterfaceJeu1.glade");

    GtkBuilder *builderJeu = gtk_builder_new_from_file(path);
    winJeu = GTK_WIDGET(gtk_builder_get_object(builderJeu, "app_jeu"));
    gtk_widget_show(winJeu);
    g_signal_connect(G_OBJECT(winJeu), "destroy",(GCallback) on_window_main_destroy, NULL);
    gtk_widget_hide(window);

    //GtkWidget *entryPseudo;
    //entryPseudo = GTK_WIDGET(gtk_builder_get_object(builder, "entryPseudo"));
    labelPseudo = GTK_WIDGET(gtk_builder_get_object(builderJeu, "lblPseudoDisplay"));
    gtk_label_set_text(GTK_LABEL(labelPseudo), (const gchar* ) tmp);

    labelBet = GTK_WIDGET(gtk_builder_get_object(builderJeu, "lblBet"));
    sprintf(tmpBet, "Il y'a %d€ en jeu", bet);
    gtk_label_set_text(GTK_LABEL(labelBet), (const gchar* ) tmpBet);
}

void on_btnCooperate_clicked(GtkButton *b){
}

void on_entryPseudo_changed(GtkEntry *e){
    sprintf(tmp,"%s, que décidez-vous de faire ?", gtk_entry_get_text(e));
    //gtk_label_set_text(GTK_LABEL(labelPseudo), (const gchar*) tmp);
    printf("%s", tmp);

}

void on_window_main_destroy()
{
    gtk_main_quit();
}