#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
 
 using namespace std;


int main(){
    int jobs,servers,custo_fixo,custo_local=0,custo_solução=0;
    ifstream arquivo;
    arquivo.open("n5m15A.txt");
    arquivo>>jobs>>servers>>custo_fixo;

    int cap_servidor[servers];

    for(int i=0;i<servers;i++){
        arquivo>>cap_servidor[i];
    }
//cria matrizes
    int matriz_custo[servers][jobs];
    int matriz_tempo[servers][jobs];
//preenche a matriz de tempo
    for(int i=0;i<servers;i++){
        for(int j=0;j<jobs;j++){
            arquivo>>matriz_tempo[i][j];
        }
    }
//preenche a matriz de custos
        for(int i=0;i<servers;i++){
        for(int j=0;j<jobs;j++){
            arquivo>>matriz_custo[i][j];
        }
    }

for(int i=0;i<jobs;i++){
        int menor_tempo=matriz_tempo[0][i];
        int aux=0;
        int aux2=0;
    for(int j=1;j<servers;j++){
        if(menor_tempo>matriz_tempo[j][i]){
            menor_tempo=matriz_tempo[j][i];
            aux=j;
            aux2=i;
        }
    }

    for(int k=0;k<servers;k++){
        if(menor_tempo>cap_servidor[k]){
            continue;
        }
        cap_servidor[k]-=menor_tempo;
    }

    custo_solução+=matriz_custo[aux][aux2];
}


for(int i=0;i<servers;i++){
    cout<<cap_servidor[i]<<'\t';
}
cout<<custo_local<<'\n'<<custo_solução;
    
}
