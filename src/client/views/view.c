#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "view.h"

#include "../../common/utils/editconf.h"
#include "../../common/protocol/protocol.h"
GtkBuilder *builder = NULL;
GtkBuilder *builderJeu = NULL;
GtkWidget *window;
GtkWidget *winJeu;
char tmp[128];
char tmpRes[128];
// int bet=50;
GtkWidget *labelPseudo;
GtkWidget *labelResult;

#define MAX_SOCK_SIZE 1024

int elapsed_time = 0;
int timer_id = 0;

packet *packetd;
char *bufferOut = NULL;
char bufferIn[MAX_SOCK_SIZE] = {0};
int sock = 0;

void init_main_window(int argc, char **argv, int sockfd)
{
    // const char * game_path = getenv( "PWD" );
    // char path[128];
    // strcpy(path, game_path);
    // strcat(path, "/src/client/glade/Interface.glade");

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("src/client/glade/Interface.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "app_win"));
    g_signal_connect(G_OBJECT(window), "destroy", (GCallback)on_window_main_destroy, NULL);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(window);

    sock = sockfd;

    packetd = init_packet(packetd, 0);
    // Parse in bufferOut
    bufferOut = set_parse(*packetd);

    send(sock, bufferOut, strlen(bufferOut), 0);
    printf("Hello message sent\n");

    read(sock, bufferIn, MAX_SOCK_SIZE);
    printf("%s\n", bufferIn);
    *packetd = get_parse(bufferIn);

    gtk_main();
}

void init_second_window()
{
    builderJeu = gtk_builder_new_from_file("src/client/glade/InterfaceJeu1.glade");
    winJeu = GTK_WIDGET(gtk_builder_get_object(builderJeu, "app_jeu"));
    gtk_widget_show(winJeu);
    g_signal_connect(G_OBJECT(winJeu), "destroy", (GCallback)on_window_main_destroy, NULL);
    gtk_builder_connect_signals(builderJeu, NULL);
    gtk_widget_hide(window);

    labelPseudo = GTK_WIDGET(gtk_builder_get_object(builderJeu, "lblPseudoDisplay"));
    gtk_label_set_text(GTK_LABEL(labelPseudo), (const gchar *)tmp);

    labelResult = GTK_WIDGET(gtk_builder_get_object(builderJeu, "lblResult"));

    read(sock, bufferIn, MAX_SOCK_SIZE);
    printf("%s\n", bufferIn);
    *packetd = get_parse(bufferIn);

    if (packetd->action_id == NOT_YOUR_TURN)
    {
        read(sock, bufferIn, MAX_SOCK_SIZE);
        printf("%s\n", bufferIn);
        *packetd = get_parse(bufferIn);
    }

    // labelBet = GTK_WIDGET(gtk_builder_get_object(builderJeu, "lblBet"));
    // sprintf(tmpBet, "Il y'a %d€ en jeu", packetd.);
    // gtk_label_set_text(GTK_LABEL(labelBet), (const gchar* ) tmpBet);
}

int timer_handler()
{
    elapsed_time++;
    char txt[100];
    // printf("timer running, time : %d\n", elapsed_time);
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

    if (timer_id == 0)
    {
        timer_id = g_timeout_add(1000, (GSourceFunc)timer_handler, NULL);
    }
    else
    {
        g_source_remove(timer_id);
        timer_id = 0;
    }

    packetd->action_id = PLAY;
    bufferOut = set_parse(*packetd);
    send(sock, bufferOut, strlen(bufferOut), 0);

    read(sock, bufferIn, MAX_SOCK_SIZE);
    printf("%s\n", bufferIn);
    *packetd = get_parse(bufferIn);

    if (packetd->action_id == YOUR_TURN || packetd->action_id == NOT_YOUR_TURN)
    {
        init_second_window();
    }
}

void on_btnCooperate_clicked(GtkButton *b)
{

    if (packetd->action_id == YOUR_TURN)
    {
        packetd->action_id = COOP;
        bufferOut = set_parse(*packetd);
        send(sock, bufferOut, strlen(bufferOut), 0);

        read(sock, bufferIn, MAX_SOCK_SIZE);

        read(sock, bufferIn, MAX_SOCK_SIZE);
        printf("%s\n", bufferIn);
        *packetd = get_parse(bufferIn);
        labelResult = GTK_WIDGET(gtk_builder_get_object(builderJeu, "lblResult"));
        switch (packetd->action_id)
        {
        case UPDATE:
            // TODO METTRE MESSAGE WIN/LOSE et EARNED MONEY
            switch (packetd->result_id)
            {
            case WIN:
                // AFFICHER MESSAGE WIN
                sprintf(tmpRes, "Vous avez gagnez : %u€", packetd->earned_money);
                gtk_label_set_text(GTK_LABEL(labelResult), (const gchar *)tmpRes);
                break;
            case LOSE:
                // AFFICHER LOSE
                break;
            default:
                break;
            }
            break;
        case FINISH:
            // Last round;
            switch (packetd->result_id)
            {
            case WIN:

                break;

            case LOSE:

                break;
            default:
                break;
            }
            break;
        case RESULTS:

            switch (packetd->result_id)
            {
            case WIN:

                // Put gameId to 0
                setID(0, ID_FILE);

                exit(0);
                break;

            case LOSE:

                // Put gameId to 0
                setID(0, ID_FILE);

                exit(0);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}
void on_btnBetray_clicked(GtkButton *b)
{
    if (packetd->action_id == YOUR_TURN)
    {
        packetd->action_id = BETRAY;
        bufferOut = set_parse(*packetd);
        send(sock, bufferOut, strlen(bufferOut), 0);
    }
}
void on_entryPseudo_changed(GtkEntry *e)
{
    sprintf(tmp, "%s, que décidez-vous de faire ?", gtk_entry_get_text(e));
    // gtk_label_set_text(GTK_LABEL(labelPseudo), (const gchar*) tmp);
}

void on_window_main_destroy()
{
    gtk_main_quit();
}