#include <iostream>
#include "vizi.h"
#include <climits>
#include "servidor_job.h"
#include "util.h"
using namespace std;

int vizi::moverJob(servidor servidores[], int num_servers, int **matriz_tempo, int **matriz_custo, int &custo_solucao) {
    int melhor_solucao = custo_solucao;
    int servidor_atual = 0;
    int aux_swap2 = 0;
    job* job_ = nullptr;
    bool troca_realizada = false;
    
    for (int s_atual = 0; s_atual < num_servers; ++s_atual) {
        
        job *job_atual = servidores[s_atual].primeiro_job;
        while (job_atual != nullptr) {
           

           
            job *proximo_job = job_atual->next_job;

            for (int s_destino = s_atual+1; s_destino < num_servers; ++s_destino) {
                if (s_destino != s_atual) {
                   

                    // Verificar se o servidor destino tem capacidade para alocar o job
                    if (matriz_tempo[s_destino][job_atual->job_id] <= servidores[s_destino].capacidade) {
                       

                        // Calcular o custo atual e o custo com a mudança
                        int custo_atual = custo_solucao - job_atual->custo;
                        int novo_custo = custo_atual + matriz_custo[s_destino][job_atual->job_id];

                        

                        // Se o novo custo for menor, mover o job
                        if (novo_custo < melhor_solucao) {
                            melhor_solucao = novo_custo;

                            servidor_atual = s_atual;
                            aux_swap2 = s_destino;
                            job_ = job_atual;
                            troca_realizada = true;

                        } 
                    } 
                }
            }

            job_atual = proximo_job; // Avançar para o próximo job no servidor atual
        }
    }
    if(!troca_realizada){
        return custo_solucao;
    }
                            // Remover o job do servidor atual
                            job *temp = servidores[servidor_atual].primeiro_job;
                            if (temp == job_) {
                                servidores[servidor_atual].primeiro_job = job_->next_job;
                            } else {
                                while (temp->next_job != job_) {
                                    temp = temp->next_job;
                                }
                                temp->next_job = job_->next_job;
                            }

                            // Adicionar o job ao servidor destino e atualiza seus valores
                            job_->next_job = servidores[aux_swap2].primeiro_job;
                            job_->tempo=matriz_tempo[aux_swap2][job_->job_id];
                            job_->custo=matriz_custo[aux_swap2][job_->job_id];

                            servidores[aux_swap2].primeiro_job = job_;
                            servidores[aux_swap2].qtd_jobs++;
                            servidores[aux_swap2].custo_T+=matriz_custo[aux_swap2][job_->job_id];
                            servidores[aux_swap2].capacidade -= matriz_tempo[aux_swap2][job_->job_id];


                            servidores[servidor_atual].qtd_jobs--;
                            servidores[servidor_atual].custo_T-=matriz_custo[servidor_atual][job_->job_id];
                            servidores[servidor_atual].capacidade += matriz_tempo[servidor_atual][job_->job_id];

                              // Atualizar a capacidade dos servidores e o custo total da solução
                            custo_solucao = melhor_solucao;
    
    return custo_solucao;

}

int vizi::Swap_Servidores(servidor servidores[], int num_servers, int **matriz_tempo, int **matriz_custo, int &custo_solucao){
    int menor_custo=INT_MAX;
    int aux_swap1=0;
    int aux_swap2=0;
    int melhor_solucao=custo_solucao;
    bool troca_feita=false;

    job* job_atual=nullptr;
    for(int i = 0; i<num_servers-1; i++){

        int Tempo_Prox_Server=0;
        int Tempo_Ant_Server=0;
        int Custo_Prox_Server=0;
        int Custo_Ant_Server=0;

        for(int j = i+1; j<num_servers; j++){
            

             job_atual=servidores[i].primeiro_job;

            while(job_atual!=nullptr)
            {
                Tempo_Prox_Server+=matriz_tempo[j][job_atual->job_id];
                Custo_Prox_Server+=matriz_custo[j][job_atual->job_id];
                job_atual=job_atual->next_job;
            }


            job_atual=servidores[j].primeiro_job;
            while(job_atual!=nullptr)
            {
                Tempo_Ant_Server += matriz_tempo[i][job_atual->job_id];
                Custo_Ant_Server += matriz_custo[i][job_atual->job_id];
                job_atual=job_atual->next_job;
            }




            if(Tempo_Prox_Server>servidores[j].capacidade_maxima||Tempo_Ant_Server>servidores[i].capacidade_maxima){
                continue;
            }
            int soma_custos_atual = servidores[i].custo_T+servidores[j].custo_T;
            int nova_soma_custos = Custo_Prox_Server+Custo_Ant_Server;

            if(soma_custos_atual < nova_soma_custos){
                continue;
            }
            if(menor_custo > nova_soma_custos){
                menor_custo = nova_soma_custos;
                aux_swap1 = i;
                aux_swap2 = j;
                troca_feita = true;
                melhor_solucao-= soma_custos_atual;
                melhor_solucao+= nova_soma_custos;
            }
        }
    }
    if(!troca_feita){
        return custo_solucao;
    }
        job_atual=servidores[aux_swap1].primeiro_job;
        servidores[aux_swap1].primeiro_job = servidores[aux_swap2].primeiro_job;
        servidores[aux_swap2].primeiro_job = job_atual;


        servidores[aux_swap1].capacidade = servidores[aux_swap1].capacidade_maxima;
        servidores[aux_swap2].capacidade = servidores[aux_swap2].capacidade_maxima;

        servidores[aux_swap1].qtd_jobs = 0;
        servidores[aux_swap2].qtd_jobs = 0;

        servidores[aux_swap1].custo_T = 0;
        servidores[aux_swap2].custo_T = 0;

        job_atual = servidores[aux_swap1].primeiro_job;
        while (job_atual!=nullptr)
        {
            job_atual->custo=matriz_custo[aux_swap1][job_atual->job_id];
            job_atual->tempo=matriz_tempo[aux_swap1][job_atual->job_id];
            servidores[aux_swap1].capacidade-=job_atual->tempo;
            servidores[aux_swap1].custo_T+=job_atual->custo;
            servidores[aux_swap1].qtd_jobs++;
            job_atual=job_atual->next_job;
        }
        job_atual = servidores[aux_swap2].primeiro_job;
        while (job_atual!=nullptr)
        {
            job_atual->custo=matriz_custo[aux_swap2][job_atual->job_id];
            job_atual->tempo=matriz_tempo[aux_swap2][job_atual->job_id];
            servidores[aux_swap2].capacidade-=job_atual->tempo;
            servidores[aux_swap2].custo_T+=job_atual->custo;
            servidores[aux_swap2].qtd_jobs++;
            job_atual=job_atual->next_job;
        }
        custo_solucao = melhor_solucao;

        return custo_solucao;

}

int vizi::SwapJobs(servidor servidores[], int num_servers, int **matriz_tempo, int **matriz_custo, int &custo_solucao) {
   
    int melhor_solucao = custo_solucao;
    int solucao_temp;
    bool troca_feita = false;
    int aux_swap1 = 0; // Guarda o servidor origem 
    int aux_swap2 = 0; // Guarda o servidor destino

    job* ptr_origem = nullptr;
    job* ptr_destino = nullptr;

    for (int i = 0; i < num_servers - 1; i++) {
        job* ptr_origem_aux = servidores[i].primeiro_job;
        for(int k = 0; k < servidores[i].qtd_jobs; k++){
            for (int j = i + 1; j < num_servers; j++){
                job* ptr_destino_aux = servidores[j].primeiro_job;
                for(int l = 0; l < servidores[j].qtd_jobs; l++){
                    
                    
                    if  (matriz_tempo[j][ptr_origem_aux->job_id] > servidores[j].capacidade + ptr_destino_aux->tempo
                        || matriz_tempo[i][ptr_destino_aux->job_id] > servidores[i].capacidade + ptr_origem_aux->tempo){
                            ptr_destino_aux = ptr_destino_aux->next_job;
                            continue;
                        }

                    
                    solucao_temp = custo_solucao;
                    solucao_temp -= (matriz_custo[i][ptr_origem_aux->job_id] + matriz_custo[j][ptr_destino_aux->job_id]);
                    solucao_temp += (matriz_custo[j][ptr_origem_aux->job_id] + matriz_custo[i][ptr_destino_aux->job_id]);
                    
                    if (solucao_temp < melhor_solucao){
                        melhor_solucao = solucao_temp;
                        troca_feita = true;
                        ptr_origem = ptr_origem_aux;
                        ptr_destino = ptr_destino_aux;
                        aux_swap1 = i;
                        aux_swap2 = j;
                    }
                    
                    ptr_destino_aux = ptr_destino_aux->next_job;
                }
            }
            ptr_origem_aux = ptr_origem_aux->next_job;
        }
    }
    // Trocar de verdade verdadeiro
    if(troca_feita){
        
        
        // Encontrar os jobs anteriores aos jobs que serão trocados
        job *anterior_origem = nullptr;
        job *anterior_destino = nullptr;
        job *atual = servidores[aux_swap1].primeiro_job;

        // Encontrar o job anterior ao ptr_origem
        while (atual != nullptr && atual->next_job != ptr_origem) {
            atual = atual->next_job;
        }
        if (atual != nullptr) {
            anterior_origem = atual;
        }

        atual = servidores[aux_swap2].primeiro_job;

        // Encontrar o job anterior ao ptr_destino
        while (atual != nullptr && atual->next_job != ptr_destino) {
            atual = atual->next_job;
        }
        if (atual != nullptr) {
            anterior_destino = atual;
        }

        // Atualizar os ponteiros dos jobs anteriores
        if (anterior_origem != nullptr) {
            anterior_origem->next_job = ptr_destino;
        } else {
            servidores[aux_swap1].primeiro_job = ptr_destino;
        }

        if (anterior_destino != nullptr) {
            anterior_destino->next_job = ptr_origem;
        } else {
            servidores[aux_swap2].primeiro_job = ptr_origem;
        }

        // Trocar os jobs entre os servidores
        job *temp = ptr_origem->next_job;
        ptr_origem->next_job = ptr_destino->next_job;
        ptr_destino->next_job = temp;
        
        custo_solucao = melhor_solucao;

        //Atualizar o Job e as infos do servidor origem
        servidores[aux_swap1].capacidade += matriz_tempo[aux_swap1][ptr_origem->job_id];
        servidores[aux_swap1].custo_T -= matriz_custo[aux_swap1][ptr_origem->job_id];
    
        ptr_destino->custo = matriz_custo[aux_swap1][ptr_destino->job_id];
        ptr_destino->tempo = matriz_tempo[aux_swap1][ptr_destino->job_id];
        servidores[aux_swap1].capacidade -= matriz_tempo[aux_swap1][ptr_destino->job_id]; 
        servidores[aux_swap1].custo_T += matriz_custo[aux_swap1][ptr_destino->job_id];
    
        // Atualizar o Job e as infos do servidor destino
        servidores[aux_swap2].capacidade += matriz_tempo[aux_swap2][ptr_destino->job_id];
        servidores[aux_swap2].custo_T -= matriz_custo[aux_swap2][ptr_destino->job_id];
        ptr_origem->custo = matriz_custo[aux_swap2][ptr_origem->job_id];
        ptr_origem->tempo = matriz_tempo[aux_swap2][ptr_origem->job_id];
        servidores[aux_swap2].capacidade -= matriz_tempo[aux_swap2][ptr_origem->job_id];
        servidores[aux_swap2].custo_T += matriz_custo[aux_swap2][ptr_origem->job_id];

    }

    return melhor_solucao;
}

int vizi::Swap2Jobs(servidor servidores[], int num_servers, int **matriz_tempo, int **matriz_custo, int &custo_solucao) {
    int melhor_solucao = custo_solucao;
    int solucao_temp = 0;
    bool troca_feita = false;
    int aux_swap1 = 0; 
    int aux_swap2 = 0; 
   
    job* ptr_origem = nullptr;
    job* ptr_origem2 = nullptr;
    job* ptr_destino = nullptr;
    job* ptr_destino2 = nullptr;
    job* ptr_origem_aux = nullptr;
    job* ptr_origem_prox = nullptr;
    job* ptr_destino_aux = nullptr;
    job* ptr_destino_prox = nullptr;

    for (int i = 0; i < num_servers; i++) {
        if (servidores[i].qtd_jobs >= 2) {
            for (job* ptr_origem_aux = servidores[i].primeiro_job; ptr_origem_aux != nullptr; ptr_origem_aux = ptr_origem_aux->next_job) {
                for (job* ptr_origem_prox = ptr_origem_aux->next_job; ptr_origem_prox != nullptr; ptr_origem_prox = ptr_origem_prox->next_job) {
                    for (int j = i + 1; j < num_servers; j++) {
                        if (servidores[j].qtd_jobs >= 2) {
                            for (job* ptr_destino_aux = servidores[j].primeiro_job; ptr_destino_aux != nullptr; ptr_destino_aux = ptr_destino_aux->next_job) {
                                for (job* ptr_destino_prox = ptr_destino_aux->next_job; ptr_destino_prox != nullptr; ptr_destino_prox = ptr_destino_prox->next_job) {


                                    if (matriz_tempo[j][ptr_origem_aux->job_id] + matriz_tempo[j][ptr_origem_prox->job_id ] > servidores[j].capacidade + ptr_destino_aux->tempo + ptr_destino_prox->tempo
                                        || matriz_tempo[i][ptr_destino_aux->job_id] + matriz_tempo[i][ptr_destino_prox->job_id] > servidores[i].capacidade + ptr_origem_aux->tempo + ptr_origem_prox->tempo){
                                            continue;
                                    }



                                    solucao_temp = custo_solucao;
                                    solucao_temp -= (matriz_custo[i][ptr_origem_aux->job_id] + matriz_custo[i][ptr_origem_prox->job_id] + matriz_custo[j][ptr_destino_aux->job_id] + matriz_custo[j][ptr_destino_prox->job_id]);
                                    solucao_temp += (matriz_custo[j][ptr_origem_aux->job_id] + matriz_custo[j][ptr_origem_prox->job_id] + matriz_custo[i][ptr_destino_aux->job_id] + matriz_custo[i][ptr_destino_prox->job_id]);

                                    

                                    if(solucao_temp < melhor_solucao){

                                        melhor_solucao = solucao_temp;
                                        troca_feita = true;
                                        ptr_origem = ptr_origem_aux;
                                        ptr_origem2 = ptr_origem_prox;
                                        ptr_destino = ptr_destino_aux;
                                        ptr_destino2 = ptr_destino_prox;
                                        aux_swap1 = i;
                                        aux_swap2 = j;
                                    }


                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if(troca_feita){


        // Encontrar os jobs anteriores aos jobs que serão trocados
        job *anterior_origem = nullptr;
        job *anterior_origem2 = nullptr;
        job *anterior_destino = nullptr;
        job *anterior_destino2 = nullptr;
        job *atual = servidores[aux_swap1].primeiro_job;
        job *auxiliar = servidores[aux_swap2].primeiro_job;


        // Realocar job origem 1
        if (ptr_origem == atual){
            servidores[aux_swap1].primeiro_job = atual->next_job;

            while(auxiliar->next_job != nullptr){
               
                auxiliar = auxiliar->next_job;
            }
            auxiliar->next_job = atual;
            atual->next_job = nullptr;

        }
        else{
            while(atual != nullptr && atual != ptr_origem){
                
                anterior_origem = atual;
                atual = anterior_origem->next_job;
            }
            anterior_origem->next_job = atual->next_job;

            while(auxiliar->next_job != nullptr){
                
                auxiliar = auxiliar->next_job;
            }

            auxiliar->next_job = atual;
            atual->next_job = nullptr;
        }

        atual = servidores[aux_swap1].primeiro_job;
        auxiliar = servidores[aux_swap2].primeiro_job;


        // Realocar job origem 2
        if(ptr_origem2 == atual){
            servidores[aux_swap1].primeiro_job = atual->next_job;

            while(auxiliar->next_job != nullptr){
                    
                auxiliar = auxiliar->next_job;
            }

            auxiliar->next_job = atual;
            atual->next_job = nullptr;

        }
        else{

            while(atual != nullptr && atual != ptr_origem2){
                  
                anterior_origem2 = atual;
                atual = anterior_origem2->next_job;
            }

            anterior_origem2->next_job = atual->next_job;

            while(auxiliar->next_job != nullptr){
                   
                auxiliar = auxiliar->next_job;
            }

            auxiliar->next_job = atual;
            atual->next_job = nullptr;
        }

        atual = servidores[aux_swap2].primeiro_job;
        auxiliar = servidores[aux_swap1].primeiro_job;

        // Realoca job destino 1
        if(ptr_destino == atual){
            servidores[aux_swap2].primeiro_job = atual->next_job;

            while(auxiliar->next_job != nullptr){
                    
                auxiliar = auxiliar->next_job;
            }

            auxiliar->next_job = atual;
            atual->next_job = nullptr;
        }
        else{

            while(atual != nullptr && atual != ptr_destino){
                   
                anterior_destino = atual;
                atual = anterior_destino->next_job;
            }

            anterior_destino->next_job = atual->next_job;

            while(auxiliar->next_job != nullptr){
                
                auxiliar = auxiliar->next_job;
            }

            auxiliar->next_job = atual;
            atual->next_job = nullptr;

        }

        atual = servidores[aux_swap2].primeiro_job;
        auxiliar = servidores[aux_swap1].primeiro_job;


        // Realoca ao job destino 2
        if(ptr_destino2 == atual){
            servidores[aux_swap2].primeiro_job = atual->next_job;

            while(auxiliar->next_job != nullptr){
                    
                auxiliar = auxiliar->next_job;
            }

            auxiliar->next_job = atual;
            atual->next_job = nullptr;
        }
        else{

            while(atual != nullptr && atual != ptr_destino2){
                   
                anterior_destino2 = atual;
                atual = anterior_destino2->next_job;
            }

            anterior_destino2->next_job = atual->next_job;

            while(auxiliar->next_job != nullptr){
                    
                auxiliar = auxiliar->next_job;
            }

            auxiliar->next_job = atual;
            atual->next_job = nullptr;

        }

        

        // Atualizar os Jobs e as infos do servidor origem
        servidores[aux_swap1].capacidade += (matriz_tempo[aux_swap1][ptr_origem->job_id] + matriz_tempo[aux_swap1][ptr_origem2->job_id]);
        servidores[aux_swap1].custo_T -= (matriz_custo[aux_swap1][ptr_origem->job_id] + matriz_custo[aux_swap1][ptr_origem2->job_id]);
        ptr_destino->custo = matriz_custo[aux_swap1][ptr_destino->job_id];
        ptr_destino->tempo = matriz_tempo[aux_swap1][ptr_destino->job_id];
        ptr_destino2->custo = matriz_custo[aux_swap1][ptr_destino2->job_id];
        ptr_destino2->tempo = matriz_tempo[aux_swap1][ptr_destino2->job_id];
        servidores[aux_swap1].capacidade -= (matriz_tempo[aux_swap1][ptr_destino->job_id] + matriz_tempo[aux_swap1][ptr_destino2->job_id]);
        servidores[aux_swap1].custo_T += (matriz_custo[aux_swap1][ptr_destino->job_id] + matriz_custo[aux_swap1][ptr_destino2->job_id]);

        // Atualizar os Jobs e as infos do servidor destino
        servidores[aux_swap2].capacidade += (matriz_tempo[aux_swap2][ptr_destino->job_id] + matriz_tempo[aux_swap2][ptr_destino2->job_id]);
        servidores[aux_swap2].custo_T -= (matriz_custo[aux_swap2][ptr_destino->job_id] + matriz_custo[aux_swap2][ptr_destino2->job_id]);
        ptr_origem->custo = matriz_custo[aux_swap2][ptr_origem->job_id];
        ptr_origem->tempo = matriz_tempo[aux_swap2][ptr_origem->job_id];
        ptr_origem2->custo = matriz_custo[aux_swap2][ptr_origem2->job_id];
        ptr_origem2->tempo = matriz_tempo[aux_swap2][ptr_origem2->job_id];
        servidores[aux_swap2].capacidade -= (matriz_tempo[aux_swap2][ptr_origem->job_id] + matriz_tempo[aux_swap2][ptr_origem2->job_id]);
        servidores[aux_swap2].custo_T += (matriz_custo[aux_swap2][ptr_origem->job_id] + matriz_custo[aux_swap2][ptr_origem2->job_id]);

    
        custo_solucao = melhor_solucao;
       
    }

    return melhor_solucao;
}

int vizi::VND(servidor servidores[], int num_servers, int **matriz_tempo, int **matriz_custo, int &custo_solucao){
    bool melhorou=true;
    int melhor_atual=custo_solucao;
    int candidato_solucao=0;

    while(melhorou){
        melhorou=false;

        candidato_solucao = vizi::Swap_Servidores(servidores,num_servers,matriz_tempo,matriz_custo,custo_solucao);
            if(candidato_solucao < melhor_atual){
                melhor_atual = candidato_solucao;
                melhorou = true;
            }
        candidato_solucao = vizi::moverJob(servidores,num_servers,matriz_tempo,matriz_custo,custo_solucao);

            if(candidato_solucao < melhor_atual){
                melhor_atual = candidato_solucao;
                melhorou = true;
                
            }
        candidato_solucao = vizi::SwapJobs(servidores,num_servers,matriz_tempo,matriz_custo,custo_solucao);

            if(candidato_solucao < melhor_atual){
                melhor_atual = candidato_solucao;
                melhorou = true;
                
            }
        candidato_solucao = vizi::Swap2Jobs(servidores,num_servers,matriz_tempo,matriz_custo,custo_solucao);

            if(candidato_solucao < melhor_atual){
                melhor_atual = candidato_solucao;
                melhorou = true;
                
            }
    }
 custo_solucao=melhor_atual;
 return 0;
}



