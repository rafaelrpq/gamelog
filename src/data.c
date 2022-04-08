#include <stdio.h>
#include <stdlib.h>

#include "../inc/data.h"

static char *arquivo = "dados.dat";

Data *lerDados (int *len) {
    Data *data = NULL;
    Data *aux  = malloc (sizeof (Data));
    FILE *file = fopen (arquivo, "rb");

    *len = 0;

    if (file == NULL) {
        fprintf (stdout, "Falha ao abrir arquivo: %s.\n[ %s ] linha: %d\n", arquivo, __FILE__, (__LINE__ -2));
        return NULL;
    }

    while (fread (aux, sizeof (Data), 1, file)) {
        data = realloc (data, (*len +1) * sizeof (Data));
        data[*len] = *aux;
        ++*len;
    }

    fclose (file);
    free (aux);
    return (data);
}

int gravarDados (Data *data) {
    FILE *file = fopen (arquivo, "a+b");
    if (file == NULL) {
        fprintf (stdout, "Falha ao abrir arquivo: %s.\n[ %s ] linha: %d\n", arquivo, __FILE__, (__LINE__ -2));
        return 0;
    }

    if (!fwrite (data, sizeof (Data), 1, file)) {
        fprintf (stdout, "Falha ao gravar dados no arquivo: %s.\n[ %s ] linha: %d\n", arquivo, __FILE__, (__LINE__ -1));
    }

    fclose (file);

    return 1;
}

int atualizarDados (long pos, Data *data) {
    FILE *file = fopen (arquivo, "rb+");
    if (file == NULL) {
        fprintf (stdout, "Falha ao abrir arquivo: %s.\n[ %s ] linha: %d\n", arquivo, __FILE__, (__LINE__ -2));
        return 0;
    }

    fseek (file, (pos) * sizeof (Data), SEEK_SET);
    if (!fwrite (data, sizeof (Data), 1, file)) {
        fprintf (stdout, "Falha ao gravar dados no arquivo: %s.\n[ %s ] linha: %d\n", arquivo, __FILE__, (__LINE__ -1));
    }

    fclose (file);

    return 1;
}

int removerDados (long pos) {
    Data *data = malloc (sizeof (Data));
    FILE *file = fopen (arquivo, "rb");
    if (file == NULL) {
        fprintf (stdout, "Falha ao abrir arquivo: %s.\n[ %s ] linha: %d\n", arquivo, __FILE__, (__LINE__ -2));
        return 0;
    }

    FILE *tmp = fopen ("tmp.dat", "wb");
    if (file == NULL) {
        fprintf (stdout, "Falha ao abrir arquivo: %s.\n[ %s ] linha: %d\n", arquivo, __FILE__, (__LINE__ -2));
        return 0;
    }

    long i = 0;
    int ret = 0;
    while (fread (data, sizeof (Data), 1, file)) {
        if (pos != i) {
            ret = fwrite (data, sizeof (Data), 1, tmp);
        }
        i++;
    }

    fclose (file);
    fclose (tmp);
    if (ret ||  (!ret && pos == 0)) {
        remove (arquivo);
        rename ("tmp.dat", arquivo);
        ret = 1;
    }
    return ret;
}

Data *buscarDados (long pos) {
    Data *data = malloc (sizeof (Data));
    FILE *file = fopen (arquivo, "rb");
    if (file == NULL) {
        fprintf (stdout, "Falha ao abrir arquivo: %s.\n[ %s ] linha: %d\n", arquivo, __FILE__, (__LINE__ -2));
        return NULL;
    }

    fseek (file, (pos) * sizeof (Data), SEEK_SET);
    fread (data, sizeof (Data), 1, file);

    return data;
}
