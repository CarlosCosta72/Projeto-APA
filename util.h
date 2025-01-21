#ifndef UTIL_H
#define UTIL_H
#include "servidor_job.h"
#include <iostream>
class util{

    public:
       static void imprimirJobs(servidor& s);
       static void gerar_Arquivo(const std::string nome_arquivo,int custo_solucao,
                                            servidor servidores[],int num_servers,servidor& servidor_local);
};


#endif