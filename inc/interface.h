#ifndef INTERFACE_H
#define INTERFACE_H

typedef struct MainWindow {
    GtkWidget *window;
    GtkWidget *btnAdicionar;
    GtkWidget *btnEditar;
    GtkWidget *btnRemover;
    GtkWidget *treeView;

    int *index;
} MainWindow;

typedef struct GameWindow {
    GtkWidget     *window;
    GtkWidget     *header;
    GtkWidget     *btnSalvar;
    GtkWidget     *game;
    GtkWidget     *console;
    GtkWidget     *log;
    GtkTextBuffer *buffer;
} GameWindow;

typedef struct DialogWindow {
    GtkWidget *window;
    GtkWidget *dlgButton;
    GtkWidget *dlgLabel;
    GtkWidget *dlgImage;
} DialogWindow;

typedef struct ConfirmWindow {
    GtkWidget *window;
    GtkWidget *cfmCancelar;
    GtkWidget *cfmRemover;
} ConfirmWindow;

typedef struct Application {
    MainWindow *mainWin;
    GameWindow *gameWin;
} Application;

MainWindow *buildMain (GtkBuilder *builder) ;

GameWindow *buildGame (GtkBuilder *builder) ;

DialogWindow *buildDialog (GtkBuilder *builder) ;

ConfirmWindow *buildConfirm (GtkBuilder *builder) ;

Application *buildApp (GtkBuilder *builder) ;

void showDialog (char *texto, char* icon) ;

void showConfirm (Application *app) ;

void cfmCancelarClicked (GtkWidget *btn, ConfirmWindow *confirmWin) ;

void cfmRemoverClicked (GtkWidget *btn, Application *app) ;

void dlgButtonClicked (GtkWidget *btn, DialogWindow *dialogWin) ;

void btnSalvarClicked (GtkWidget *btn, Application *app) ;

void btnAdicionarClicked (GtkWidget *btn, Application *app) ;

void btnEditarClicked (GtkWidget *btn, Application *app) ;

void btnRemoverClicked (GtkWidget *btn, Application *app) ;

void definirColunas (GtkCellRenderer *render, GtkWidget *treeView) ;

void preencherTabela (GtkWidget *treeView) ;

void treeViewGetIndices (GtkWidget *treeView, GtkTreePath *path, GtkTreeViewColumn *column, MainWindow *mainWin) ;

void on_activate (GtkApplication *gApp) ;

#endif
