/**
 * @file view.c
 * @author KVEP - https://gitlab.com/kvep/
 * @version 1.0
 * @date 2021-11-17
 * 
 * @copyright Copyright (c) 2021 KVEP
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "view.h"

#include "../../common/utils/editconf.h"
#include "../../common/protocol/protocol.h"
#include "../socket/clientcxn.h"
#define MAX_SOCK_SIZE 1024
GtkBuilder *builder = NULL;
GtkBuilder *builderJeu = NULL;
GtkWidget *window;
GtkWidget *winJeu;
char tmp[128];
char tmpRes[128];
// int bet=50;
GtkWidget *labelPseudo;
GtkWidget *labelResult;
GtkWidget *labelCoop;
GtkWidget *labelBetray;
GtkLabel *timelabel;
GtkWidget *prog1;
int countCurrent = 5;
int countDownMax = 10;

int elapsed_time = TIME_FOR_ROUND;
int timer_id = 0;

packet *packetd;
char *bufferOut = NULL;
char bufferIn[MAX_SOCK_SIZE] = {0};
int sock = 0;
pthread_t thread;
int second_window = 0;

/**
 * @brief starts and displays the first graphic window (homepage window)
 * @param argc nothing
 * @param argv nothing
 * @param sockfd connection socket
 */
void init_main_window(int argc, char **argv, int sockfd)
{
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

/**
 * @brief starts and displays the game graphic window, same for each round during game.
 * This window contain both choices (betray or coop) 
 */
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

    GtkButton *coopButton = GTK_BUTTON(gtk_builder_get_object(builderJeu, "btnCooperate"));
    GtkButton *betrayButton = GTK_BUTTON(gtk_builder_get_object(builderJeu, "btnBetray"));
    labelCoop = GTK_WIDGET(gtk_builder_get_object(builderJeu, "lblCooperate"));
    labelResult = GTK_WIDGET(gtk_builder_get_object(builderJeu, "lblResult"));
    labelBetray = GTK_WIDGET(gtk_builder_get_object(builderJeu, "lblBetray"));
    switch (packetd->action_id)
    {
    case NOT_YOUR_TURN:
        sprintf(tmpRes, "Wait for your turn !");
        gtk_label_set_text(GTK_LABEL(labelResult), (const gchar *)tmpRes);
        gtk_widget_hide(GTK_WIDGET(coopButton));
        gtk_widget_hide(GTK_WIDGET(betrayButton));
        gtk_widget_hide(GTK_WIDGET(labelCoop));
        gtk_widget_hide(GTK_WIDGET(labelBetray));
        break;
    case YOUR_TURN:
        sprintf(tmpRes, "It's your turn to play");
        gtk_label_set_text(GTK_LABEL(labelResult), (const gchar *)tmpRes);
        gtk_widget_show(GTK_WIDGET(coopButton));
        gtk_widget_show(GTK_WIDGET(betrayButton));
        break;

    default:
        break;
    }

    pthread_create(&thread, 0, threadProcess, &sock);
    pthread_detach(thread);

}

/**
 * @brief function to update timer of client
 * @return int : code
 */
int timer_handler()
{
    if (packetd->action_id == YOUR_TURN)
    {

        if (elapsed_time > 0)
        {
            elapsed_time--;
            char txt[100];
            //printf("timer running, time : %d\n", elapsed_time);
            timelabel = GTK_LABEL(gtk_builder_get_object(builderJeu, "time_display"));
            snprintf(txt, 100, "%04i", elapsed_time);
            gtk_label_set_text(timelabel, txt);
        }
        else
        {
            packetd->action_id = NO_RESPONSE;
            bufferOut = set_parse(*packetd);
            send(sock, bufferOut, strlen(bufferOut), 0);

        }
    }
    return 1;
}

/**
 * @brief when the 'play' button is clicked this function close the first graphic window and open the second
 * @param b gtk button 
 */
void on_buttonPlay_clicked(GtkButton *b)
{
    if (timer_id == 0)
    {
        timer_id = g_timeout_add(1000, (GSourceFunc)timer_handler, NULL);
    }else
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
        setID(packetd->game_id, CLIENT_PATH);
    }
}

/**
 * @brief sends the choice (cooperate) to the server 
 * @param b gtk button
 */
void on_btnCooperate_clicked(GtkButton *b)
{

    if (packetd->action_id == YOUR_TURN)
    {
        packetd->action_id = COOP;
        packetd->time = (u_int16_t)  (5 - elapsed_time);
        bufferOut = set_parse(*packetd);
        send(sock, bufferOut, strlen(bufferOut), 0);

        // gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(prog1), (gdouble) 1.00);
        //  g_source_remove(timer_id);
        //  elapsed_time = TIME_FOR_ROUND;
    }
}

/**
 * @brief sends the choice (betray) to the server
 * @param b gtk button
 */
void on_btnBetray_clicked(GtkButton *b)
{
    if (packetd->action_id == YOUR_TURN)
    {
        packetd->action_id = BETRAY;
        packetd->time = (u_int16_t)  (5 - elapsed_time);
        bufferOut = set_parse(*packetd);
        send(sock, bufferOut, strlen(bufferOut), 0);

        // g_source_remove(timer_id);
        // elapsed_time = TIME_FOR_ROUND;
    }
}

/**
 * @brief creates temp variable with the pseudo choose in the first window
 * @param e pseudo
 */
void on_entryPseudo_changed(GtkEntry *e)
{
    sprintf(tmp, "%s, what do you decide to do ?", gtk_entry_get_text(e));
    // gtk_label_set_text(GTK_LABEL(labelPseudo), (const gchar*) tmp);
}

/**
 * @brief closes the gtk window
 */
void on_window_main_destroy()
{
    gtk_main_quit();
}

/**
 * @brief updates graphic window during all the game
 * @param packetReceived packet delivered by the server with all information
 */
void update_view(packet packetReceived)
{
    packetd->action_id = packetReceived.action_id;
    packetd->client_id = packetReceived.client_id;
    packetd->current_round = packetReceived.current_round;
    packetd->earned_money = packetReceived.earned_money;
    packetd->game_id = packetReceived.game_id;
    packetd->result_id = packetReceived.result_id;
    packetd->time = packetReceived.time;

    GtkButton *coopButton = GTK_BUTTON(gtk_builder_get_object(builderJeu, "btnCooperate"));
    GtkButton *betrayButton = GTK_BUTTON(gtk_builder_get_object(builderJeu, "btnBetray"));

    labelResult = GTK_WIDGET(gtk_builder_get_object(builderJeu, "lblResult"));
    switch (packetd->action_id)
    {
    case NOT_YOUR_TURN:
        sprintf(tmpRes, "Not your turn");
        gtk_label_set_text(GTK_LABEL(labelResult), (const gchar *)tmpRes);
        gtk_widget_hide(GTK_WIDGET(coopButton));
        gtk_widget_hide(GTK_WIDGET(betrayButton));
        gtk_widget_hide(GTK_WIDGET(labelCoop));
        gtk_widget_hide(GTK_WIDGET(labelBetray));
        break;
    case YOUR_TURN:
        elapsed_time = TIME_FOR_ROUND;
        sprintf(tmpRes, "Your turn");
        gtk_label_set_text(GTK_LABEL(labelResult), (const gchar *)tmpRes);
        gtk_widget_show(GTK_WIDGET(coopButton));
        gtk_widget_show(GTK_WIDGET(betrayButton));
        break;
    case UPDATE:
        gtk_widget_hide(GTK_WIDGET(coopButton));
        gtk_widget_hide(GTK_WIDGET(betrayButton));
        // TODO METTRE MESSAGE WIN/LOSE et EARNED MONEY
        switch (packetd->result_id)
        {
        case WIN:
            // AFFICHER MESSAGE WIN
            sprintf(tmpRes, "You won : %u€ the previous round", packetd->earned_money);
            gtk_label_set_text(GTK_LABEL(labelResult), (const gchar *)tmpRes);
            break;
        case LOSE:
            // AFFICHER LOSE
            sprintf(tmpRes, "You lost the previous round");
            gtk_label_set_text(GTK_LABEL(labelResult), (const gchar *)tmpRes);
            break;
        default:
            break;
        }
        break;
    case FINISH:
        gtk_widget_hide(GTK_WIDGET(coopButton));
        gtk_widget_hide(GTK_WIDGET(betrayButton));
        // Last round;
        switch (packetd->result_id)
        {
        case WIN:
            sprintf(tmpRes, "Its the last round ! You won : %u€ the last round", packetd->earned_money);
            gtk_label_set_text(GTK_LABEL(labelResult), (const gchar *)tmpRes);
            break;
        case LOSE:
            sprintf(tmpRes, "Its the last round ! You have lost");
            gtk_label_set_text(GTK_LABEL(labelResult), (const gchar *)tmpRes);
            break;
        default:
            break;
        }
        break;
    case RESULTS:
        gtk_widget_hide(GTK_WIDGET(coopButton));
        gtk_widget_hide(GTK_WIDGET(betrayButton));
        switch (packetd->result_id)
        {
        case WIN:
            sprintf(tmpRes, "You won the the game and %u€", packetd->earned_money);
            gtk_label_set_text(GTK_LABEL(labelResult), (const gchar *)tmpRes);
            // Put gameId to 0
            setID(0, ID_FILE);
            break;

        case LOSE:
            sprintf(tmpRes, "You lost the game but you won %u€", packetd->earned_money);
            gtk_label_set_text(GTK_LABEL(labelResult), (const gchar *)tmpRes);
            // Put gameId to 0
            setID(0, ID_FILE);
            break;
        default:
            break;
        }
        // wait 10 secondes to show results
        sleep(10);
        close(sock);
        exit(0);
        break;
    default:
        break;
    }
}
