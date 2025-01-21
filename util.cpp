#include <iostream>
#include <fstream>
#include "util.h"
#include "servidor_job.h"


using namespace std;

void util::imprimirJobs(servidor& s) {
    cout << "Servidor ID: " << s.servidor_id << endl;
    cout << "Capacidade restante: " << s.capacidade << endl;
    cout << "Custo total do servidor: "<< s.custo_T << endl;
    cout << "Numero de jobs: " << s.qtd_jobs << endl;
    cout << "Detalhes dos jobs:" << endl;

    job* atual = s.primeiro_job;
    while (atual != nullptr) {
        cout << "  Job ID: " << atual->job_id << ", Tempo: " << atual->tempo << ", Custo: " << atual->custo << endl;
        atual = atual->next_job;
    }
    cout << endl;
}
void util::gerar_Arquivo(string nome_arquivo, int custo_solucao, servidor servidores[], int num_servers, servidor& servidor_local) {

    ofstream arquivo(nome_arquivo);

    if (arquivo.is_open()) {
        // Escrever o valor da solução
        arquivo << custo_solucao << endl;

        // Calcular o custo total da alocação nos servidores
        int custo_total_servidores = 0;
        for (int i = 0; i < num_servers; i++) {
            custo_total_servidores += servidores[i].custo_T;
        }
        arquivo << custo_total_servidores << endl;

        // Escrever o custo associado aos jobs executados localmente
        arquivo << servidor_local.custo_T << endl;

        // Pular uma linha
        arquivo << endl;

        // Escrever a lista de jobs alocados em cada servidor
        for (int i = 0; i < num_servers; i++) {
            job* atual = servidores[i].primeiro_job;
            while (atual != nullptr) {
                arquivo << atual->job_id << " ";
                atual = atual->next_job;
            }
            arquivo << endl;
        }

        // Fechar o arquivo
        arquivo.close();
        cout << "Arquivo '" << nome_arquivo << "' gerado com sucesso!" << endl;
    } else {
        cout << "Erro ao criar o arquivo." << endl;
    }
    return;
}
