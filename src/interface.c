#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "../inc/interface.h"
#include "../inc/data.h"

static char *resource = "/com/github/rafaelrpq/gamelog/gamelog.ui";
// static char *image = "/com/github/rafaelrpq/gamelog/logbook.png";

char *arquivo;

MainWindow *buildMain (GtkBuilder *builder) {
    MainWindow *mainWin = malloc (sizeof (MainWindow));

    mainWin->window       = GTK_WIDGET (gtk_builder_get_object (builder, "mainWindow"));
    mainWin->btnAdicionar = GTK_WIDGET (gtk_builder_get_object (builder, "btnAdicionar"));
    mainWin->btnEditar    = GTK_WIDGET (gtk_builder_get_object (builder, "btnEditar"));
    mainWin->btnRemover   = GTK_WIDGET (gtk_builder_get_object (builder, "btnRemover"));
    mainWin->treeView     = GTK_WIDGET (gtk_builder_get_object (builder, "treeview"));

    mainWin->index = -1;

    // GtkWidget *icon = gtk_image_new_from_resource (image);
    // gtk_window_set_icon (GTK_WINDOW (mainWin->window), gtk_image_get_pixbuf (icon));

    arquivo = strcat (getenv ("HOME"), "/.config/gamelog.dat");

    GtkCellRenderer *render = NULL;
    definirColunas (render, mainWin->treeView);
    preencherTabela (mainWin->treeView);

    return mainWin;
}

GameWindow *buildGame (GtkBuilder *builder) {
    GameWindow *gameWin = malloc (sizeof (GameWindow));

    gameWin->window    = GTK_WIDGET (gtk_builder_get_object (builder, "gameWindow"));
    gameWin->header    = GTK_WIDGET (gtk_builder_get_object (builder, "header"));
    gameWin->btnSalvar = GTK_WIDGET (gtk_builder_get_object (builder, "btnSalvar"));
    gameWin->game      = GTK_WIDGET (gtk_builder_get_object (builder, "game"));
    gameWin->console   = GTK_WIDGET (gtk_builder_get_object (builder, "console"));
    gameWin->log       = GTK_WIDGET (gtk_builder_get_object (builder, "log"));

    gameWin->buffer    =  gtk_text_buffer_new (NULL);
    gtk_text_view_set_buffer (GTK_TEXT_VIEW (gameWin->log), gameWin->buffer);

    return gameWin;
}

DialogWindow *buildDialog (GtkBuilder *builder) {
    DialogWindow *dialogWin = malloc (sizeof (DialogWindow));

    dialogWin->window    = GTK_WIDGET (gtk_builder_get_object (builder, "dlgWindow"));
    dialogWin->dlgButton = GTK_WIDGET (gtk_builder_get_object (builder, "dlgButton"));
    dialogWin->dlgLabel  = GTK_WIDGET (gtk_builder_get_object (builder, "dlgLabel"));
    dialogWin->dlgImage  = GTK_WIDGET (gtk_builder_get_object (builder, "dlgImage"));

    return dialogWin;
}

ConfirmWindow *buildConfirm (GtkBuilder *builder) {
    ConfirmWindow *confirmWin = malloc (sizeof (ConfirmWindow));

    confirmWin->window      = GTK_WIDGET (gtk_builder_get_object (builder, "cfmWindow"));
    confirmWin->cfmCancelar = GTK_WIDGET (gtk_builder_get_object (builder, "cfmCancelar"));
    confirmWin->cfmRemover  = GTK_WIDGET (gtk_builder_get_object (builder, "cfmRemover"));

    return confirmWin;
}

Application *buildApp (GtkBuilder *builder) {
    Application *app = malloc (sizeof (Application));

    app->mainWin = buildMain (builder);
    gtk_tree_view_set_activate_on_single_click (GTK_TREE_VIEW (app->mainWin->treeView), TRUE);

    g_signal_connect (app->mainWin->btnAdicionar, "clicked"      , G_CALLBACK (btnAdicionarClicked), app);
    g_signal_connect (app->mainWin->btnEditar   , "clicked"      , G_CALLBACK (btnEditarClicked)   , app);
    g_signal_connect (app->mainWin->btnRemover  , "clicked"      , G_CALLBACK (btnRemoverClicked)  , app);
    g_signal_connect (app->mainWin->treeView    , "row_activated", G_CALLBACK (treeViewGetIndices) , app->mainWin);

    GtkTreePath *path;
    GtkTreeViewColumn *column;

    return app;
}

void showDialog (char *texto, char* icon) {
    GtkBuilder *builder = gtk_builder_new_from_resource (resource);
    DialogWindow *dialogWin = buildDialog (builder);
    g_signal_connect (dialogWin->dlgButton, "clicked", G_CALLBACK (dlgButtonClicked), dialogWin);
    gtk_label_set_text (GTK_LABEL (dialogWin->dlgLabel), texto);
    gtk_image_set_from_icon_name (GTK_IMAGE (dialogWin->dlgImage), icon, GTK_ICON_SIZE_DIALOG);
    gtk_widget_show_all (dialogWin->window);
}

void showConfirm (Application *app) {
   GtkBuilder *builder = gtk_builder_new_from_resource (resource);
   ConfirmWindow *confirmWin = buildConfirm (builder);
   g_signal_connect (confirmWin->cfmCancelar, "clicked", G_CALLBACK (cfmCancelarClicked), confirmWin);
   g_signal_connect (confirmWin->cfmRemover , "clicked", G_CALLBACK (cfmRemoverClicked) , app);
   g_signal_connect (confirmWin->cfmRemover , "clicked", G_CALLBACK (cfmCancelarClicked), confirmWin);
   gtk_widget_show_all (confirmWin->window);
}

void cfmCancelarClicked (GtkWidget *btn, ConfirmWindow *confirmWin) {
    gtk_widget_destroy (confirmWin->window);
}

void cfmRemoverClicked (GtkWidget *btn, Application *app) {
    if (removerDados (arquivo, (long) app->mainWin->index)) {
        showDialog ("Registro removido com sucesso!", "dialog-information");
        preencherTabela (app->mainWin->treeView);
    } else {
        showDialog ("Não foi possível remover o registro!", "dialog-error");
    }
    app->mainWin->index = -1;
}

void dlgButtonClicked (GtkWidget *btn, DialogWindow *dialogWin) {
    gtk_widget_destroy (dialogWin->window);
}

void btnSalvarClicked (GtkWidget *btn, Application *app) {
    Data *data = malloc (sizeof (Data));

    GtkTextIter start = {0,}, end = {0,};
    gtk_text_buffer_get_bounds (app->gameWin->buffer, &start, &end);

    char *game    = (char *) gtk_entry_get_text (GTK_ENTRY (app->gameWin->game));
    char *console = (char *) gtk_entry_get_text (GTK_ENTRY (app->gameWin->console));
    char *log     = gtk_text_buffer_get_text (app->gameWin->buffer, &start, &end, FALSE);

    if (strcmp (game, "") == 0 || strcmp (console, "") == 0 || strcmp (log, "") == 0) {
        showDialog ("Informe todos os campos!", "dialog-warning");
        return ;
    }

    strcpy (data->game   , game);
    strcpy (data->console, console);
    strcpy (data->log    , log);

    if (app->mainWin->index == -1) {
        if (gravarDados (arquivo, data)) {
            showDialog ("Registro salvo com sucesso!", "emblem-default");
            preencherTabela (app->mainWin->treeView);
            gtk_widget_destroy (app->gameWin->window);
        } else {
            showDialog ("Não foi possível salvar o registro!", "dialog-error");
        }
    } else {
        if (atualizarDados (arquivo, (long) app->mainWin->index, data)) {
            showDialog ("Registro atualizado com sucesso!", "emblem-default");
            preencherTabela (app->mainWin->treeView);
            gtk_widget_destroy (app->gameWin->window);
        } else {
            showDialog ("Não foi possível atualizar o registro!", "dialog-error");
        }
    }
}

void btnAdicionarClicked (GtkWidget *btn, Application *app) {
    GtkBuilder *builder = gtk_builder_new_from_resource (resource);
    app->gameWin = buildGame (builder) ;
    app->mainWin->index = -1;
    gtk_header_bar_set_title (GTK_HEADER_BAR (app->gameWin->header), "Novo Registro");
    g_signal_connect (app->gameWin->btnSalvar, "clicked", G_CALLBACK (btnSalvarClicked), app);
    gtk_widget_show_all (app->gameWin->window);
}


void btnEditarClicked (GtkWidget *btn, Application *app) {
    if (app->mainWin->index != -1) {
        GtkBuilder *builder = gtk_builder_new_from_resource (resource);
        app->gameWin = buildGame (builder) ;
        gtk_header_bar_set_title (GTK_HEADER_BAR (app->gameWin->header), "Editar Registro");

        Data *data = buscarDados (arquivo, (long) app->mainWin->index);
        gtk_entry_set_text (GTK_ENTRY (app->gameWin->game)   , data->game);
        gtk_entry_set_text (GTK_ENTRY (app->gameWin->console), data->console);
        gtk_text_buffer_set_text (GTK_TEXT_BUFFER (app->gameWin->buffer), data->log, -1);

        g_signal_connect (app->gameWin->btnSalvar, "clicked", G_CALLBACK (btnSalvarClicked), app);
        gtk_widget_show_all (app->gameWin->window);
        return ;
    }

    showDialog ("Clique no registro que deseja editar.", "dialog-information");
}

void btnRemoverClicked (GtkWidget *btn, Application *app) {
    if (app->mainWin->index != -1) {
        showConfirm (app);
        // app->mainWin->index = -1;
        return ;
    }

    showDialog ("Clique no registro que deseja remover.", "dialog-information");
    // printf ("[ Remover ] => clicado\n");
}

void definirColunas (GtkCellRenderer *render, GtkWidget *treeView) {
    render = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (
        GTK_TREE_VIEW (treeView),
       -1, "ID", render, "text", 0, NULL
    );

    render = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (
        GTK_TREE_VIEW (treeView),
       -1, "Jogo", render, "text", 1, NULL
    );

    render = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (
        GTK_TREE_VIEW (treeView),
       -1, "Console", render, "text", 2, NULL
    );
}

void preencherTabela (GtkWidget *treeView) {
    int len;
    Data *data = lerDados (arquivo, &len);
    GtkTreeIter iter = {0, };

    GtkListStore *store = gtk_list_store_new (3, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING);
    gtk_list_store_clear(store);

    gtk_tree_view_set_model (GTK_TREE_VIEW (treeView), GTK_TREE_MODEL (store));

    for (int i = 0; i < len; i++) {
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (
            store, &iter,
            0, i,
            1, data[i].game,
            2, data[i].console,
            -1
        );
    }
}

void treeViewGetIndices (GtkWidget *treeView, GtkTreePath *path, GtkTreeViewColumn *column, MainWindow *mainWin) {
    int *i = gtk_tree_path_get_indices (path);
    mainWin->index = *i;
}

void on_activate (GtkApplication *gApp) {
    GtkBuilder *builder = gtk_builder_new_from_resource (resource);
    Application *app = buildApp (builder);

    gtk_window_set_application (GTK_WINDOW (app->mainWin->window), gApp);
    gtk_widget_show_all (app->mainWin->window);
}
