#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "../inc/data.h"
#include "../inc/interface.h"

int main (int argc, char **argv) {
    GtkApplication *gApp = gtk_application_new (
        "com.github.rafaelrpq.gamelog",
         G_APPLICATION_FLAGS_NONE
    );

    g_signal_connect (gApp, "activate", G_CALLBACK (on_activate), NULL);

    return g_application_run (G_APPLICATION (gApp), argc, argv);
}
