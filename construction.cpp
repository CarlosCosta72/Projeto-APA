#include <iostream>
#include "construction.h"
#include "servidor_job.h"
#include <climits>
using namespace std;
void construction::guloso(servidor servidores[],servidor servidor_local,int num_servers,int num_jobs,int custo_fixo,
                                            int** matriz_tempo,int** matriz_custo, int& custo_solucao)
{
 for (int j = 0; j < num_jobs; j++)
    {
        int menor_tempo = INT_MAX;
        int servidor_escolhido = 0;

        // Procura o servidor com menor tempo
        for (int s = 0; s < num_servers; s++)
        {
            if (menor_tempo > matriz_tempo[s][j])
            {
                menor_tempo = matriz_tempo[s][j];
                servidor_escolhido = s;
            }
        }

   

        // Verifica se o servidor tem capacidade para o job
        if (menor_tempo <= servidores[servidor_escolhido].capacidade)
        {
            // Cria um novo job com um ID único
            job *novo_job = new job(j, matriz_tempo[servidor_escolhido][j], matriz_custo[servidor_escolhido][j]);

            // Adiciona o job à lista encadeada do servidor
            if (servidores[servidor_escolhido].primeiro_job == nullptr)
            {
                servidores[servidor_escolhido].primeiro_job = novo_job;
            }
            else
            {
                job *temp = servidores[servidor_escolhido].primeiro_job;
                while (temp->next_job != nullptr)
                {
                    temp = temp->next_job;
                }
                temp->next_job = novo_job;
            }

            // Atualiza a capacidade e o custo total
            servidores[servidor_escolhido].capacidade -= menor_tempo;
            servidores[servidor_escolhido].qtd_jobs++;
            servidores[servidor_escolhido].custo_T+=matriz_custo[servidor_escolhido][j];
            custo_solucao += matriz_custo[servidor_escolhido][j];
        }
        else
        {
            // Procura o próximo servidor com capacidade suficiente

        

            bool job_alocado = false;
            for (int s = 0; s < num_servers; s++)
            {
                if (matriz_tempo[s][j] <= servidores[s].capacidade)
                {
                    // Cria um novo job com um ID único
                    job *novo_job = new job(j, matriz_tempo[s][j], matriz_custo[s][j]);

                    // Adiciona o job à lista encadeada do servidor
                    if (servidores[s].primeiro_job == nullptr)
                    {
                        servidores[s].primeiro_job = novo_job;
                    }
                    else
                    {
                        job *temp = servidores[s].primeiro_job;
                        while (temp->next_job != nullptr)
                        {
                            temp = temp->next_job;
                        }
                        temp->next_job = novo_job;
                    }

                    // Atualiza a capacidade e o custo total
                    servidores[s].capacidade -= matriz_tempo[s][j];
                    servidores[s].qtd_jobs++;
                    servidores[s].custo_T+=matriz_custo[s][j];
                    custo_solucao += matriz_custo[s][j];

                    job_alocado = true;
                    break;
                }
            }

            // Se não conseguir alocar em nenhum servidor, aloca no "servidor local"
            if (!job_alocado)
            {
                // Cria um novo job com um ID único
                job *novo_job = new job(j, matriz_tempo[servidor_escolhido][j], custo_fixo);

                // Adiciona o job à lista encadeada do servidor local
                if (servidor_local.primeiro_job == nullptr)
                {
                    servidor_local.primeiro_job = novo_job;
                }
                else
                {
                    job *temp = servidor_local.primeiro_job;
                    while (temp->next_job != nullptr)
                    {
                        temp = temp->next_job;
                    }
                    temp->next_job = novo_job;
                }

                // Atualiza o custo total
                custo_solucao += custo_fixo;
            }
        }
    }
}