#ifndef SERVIDOR_JOB_H
#define SERVIDOR_JOB_H
struct job
{
    int job_id;
    int tempo;
    int custo;
    job *next_job;

    // Construtor para inicializar os valores padrão
    job(int id = 0, int t = 0, int c = 0) : job_id(id), tempo(t), custo(c), next_job(nullptr) {}
};

struct servidor
{
    int servidor_id;
    int qtd_jobs;
    int capacidade;
    int capacidade_maxima;
    int custo_T;
    job *primeiro_job;

    // Construtor para inicializar os valores padrão
    servidor(int id = 0, int qtd = 0, int cap = 0,int custo=0) : servidor_id(id), qtd_jobs(qtd), capacidade(cap),
                                                        capacidade_maxima(cap),custo_T(custo) ,primeiro_job(nullptr) {}
};



#endif
