#ifndef GULOSO_H
#define GULOSO_H
#include "servidor_job.h"
#include <iostream>

class construction{

    public:
        static void guloso(servidor servidores[],servidor servidor_local,int num_servers,int num_jobs,int custo_fixo,
                                            int** matriz_tempo,int** matriz_custo, int& custo_solucao);

};


#endif