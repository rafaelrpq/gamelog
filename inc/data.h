#ifndef DATA_H
#define DATA_H

typedef struct {
    char game[64];
    char console[64];
    char log[256];
} Data;

Data *lerDados (char *arquivo, int *len) ;

int gravarDados (char *arquivo, Data *data) ;

int atualizarDados (char *arquivo, long pos, Data *data) ;

int removerDados (char *arquivo, long pos) ;

Data *buscarDados (char *arquivo, long pos) ;

#endif
