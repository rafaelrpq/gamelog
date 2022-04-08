#ifndef DATA_H
#define DATA_H

typedef struct {
    char game[64];
    char console[64];
    char log[256];
} Data;

Data *lerDados (int *len) ;

int gravarDados (Data *data) ;

int atualizarDados (long pos, Data *data) ;

int removerDados (long pos) ;

Data *buscarDados (long pos) ;

#endif
