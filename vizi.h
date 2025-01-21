#ifndef VIZI_H
#define VIZI_H
#include "servidor_job.h"
#include <iostream>
class vizi{

    public:
    static int moverJob(servidor servidores[], int num_servers, int **matriz_tempo, int **matriz_custo, int &custo_solucao);
    static int Swap_Servidores(servidor servidores[], int num_servers, int **matriz_tempo, int **matriz_custo, int &custo_solucao);
    static int SwapJobs(servidor servidores[], int num_servers, int **matriz_tempo, int **matriz_custo, int &custo_solucao);
    static int Swap2Jobs(servidor servidores[], int num_servers, int **matriz_tempo, int **matriz_custo, int &custo_solucao);
    static int VND(servidor servidores[], int num_servers, int **matriz_tempo, int **matriz_custo, int &custo_solucao);

};

#endif