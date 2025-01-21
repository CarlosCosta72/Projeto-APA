#include <iostream>
#include <fstream>
#include <climits>
#include <string>
#include <chrono>
#include "servidor_job.h"
#include "construction.h"
#include "util.h"
#include "vizi.h"

using namespace std;
using namespace std::chrono;

// Função para imprimir os detalhes de todos os jobs em um servidor







int main(int argc,char* argv[])
{
    int num_jobs, num_servers, custo_fixo, custo_local = 0, custo_solucao = 0;
    ifstream arquivo;
    arquivo.open(argv[1]);
    arquivo >> num_jobs >> num_servers >> custo_fixo;

    // Array de servidores
    servidor servidores[num_servers]; 
    // Inicializa o servidor "local"
    servidor servidor_local(num_servers, 0, INT_MAX); // Capacidade ilimitada


    // Matrizes de custo e tempo (alocação dinâmica)
    int **matriz_custo = new int*[num_servers];
    int **matriz_tempo = new int*[num_servers];
    for (int i = 0; i < num_servers; ++i) {
        matriz_custo[i] = new int[num_jobs];
        matriz_tempo[i] = new int[num_jobs];
    }

    // Preenche os dados dos servidores
    for (int i = 0; i < num_servers; i++)
    {
        servidores[i].servidor_id = i;
        arquivo >> servidores[i].capacidade_maxima;
        servidores[i].capacidade=servidores[i].capacidade_maxima;
    }

    // Preenche a matriz de tempo
    for (int i = 0; i < num_servers; i++)
    {
        for (int j = 0; j < num_jobs; j++)
        {
            arquivo >> matriz_tempo[i][j];
        }
    }

    // Preenche a matriz de custos
    for (int i = 0; i < num_servers; i++)
    {
        for (int j = 0; j < num_jobs; j++)
        {
            arquivo >> matriz_custo[i][j];
        }
    }





    // Medindo o tempo do algoritmo construtivo: 
    auto start = steady_clock::now();
    construction::guloso(servidores,servidor_local,num_servers,num_jobs,custo_fixo,matriz_tempo,matriz_custo,custo_solucao);
    auto end = steady_clock::now();

    auto tempo_percorrido = end - start;
    cout << "O guloso demorou: " << tempo_percorrido.count() << "ns\n";



    cout << "Valor da solucao atual: " << custo_solucao << endl<<endl;

    start = steady_clock::now();
    vizi::VND(servidores,num_servers,matriz_tempo,matriz_custo,custo_solucao);
    end = steady_clock::now();

    tempo_percorrido = end - start;
    cout << "O VND demorou: " << tempo_percorrido.count() << "ns\n";

    cout << "Valor da solucao atual: " << custo_solucao << endl;
    cout << endl;
    for (int i = 0; i < num_servers ; i++) {
        util::imprimirJobs(servidores[i]);
    }
        util::imprimirJobs(servidor_local);
    cout << endl << custo_solucao << endl;

    util::gerar_Arquivo("solucao.txt", custo_solucao, servidores, num_servers, servidor_local);
    arquivo.close();


    return 0;
}
