#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>

using namespace std;

#define MAX 6 // maximo de elementos da fila
#define QUANTUM 2

typedef struct Processos
{
	int tChegada;
	int tPico;
	int tRestante; //para usar no RR
} Processo;


bool comparacao(Processo p1, Processo p2){
	return (p1.tPico <= p2.tPico);
}


vector <Processo> LerArquivo(FILE *arq){
	arq = fopen("numeros.txt", "rt");
	int auxChegada; //variaveis auxiliares para 
	int auxPico; 	//serem colocadas no struct

	std::vector<Processo> p;

	while(!feof(arq)){
		fscanf(arq, "%d %d", &auxChegada, &auxPico);

		if(feof(arq)) 
			break;

		Processo novo;
		novo.tChegada = auxChegada;
		novo.tPico = auxPico;
		p.push_back(novo);
	}

	fclose(arq);


	return p;

}

void FCFS(vector <Processo> filaProcessos){
	float tRetorno = 0, tResposta = 0, tEspera = 0, tExec = 0;
	int tamFila = filaProcessos.size(); //tamanho do vetor

	for(int i = 0; i < filaProcessos.size(); i++){
		Processo p = filaProcessos[i];
		
		if(p.tChegada > tExec){
			tExec = p.tChegada;
		}
	
		tRetorno += tExec + p.tPico - p.tChegada;
		tResposta += tExec - p.tChegada;
		tEspera += tExec - p.tChegada;
	
		tExec = tExec + p.tPico;
	}

	float mediaRetorno = (tRetorno/tamFila);
	float mediaResposta = (tResposta/tamFila);
	float mediaEspera = (tEspera/tamFila);

	cout << "FCFS " << mediaRetorno << " " << mediaResposta << " " << mediaEspera << endl; 

}

void SJF(vector <Processo> filaProcessos){
	float tRetorno = 0, tResposta = 0, tEspera = 0, tExec = 0;
	int tamFila = filaProcessos.size(); //tamanho do vetor
	int qProcesso = tamFila;

	vector <Processo> procProntos; //lista dos processos prontos para execução

	while(qProcesso != 0){
		for(int i = 0; i < filaProcessos.size();  i++){
			if(filaProcessos[i].tChegada <= tExec){
				procProntos.push_back(filaProcessos[i]);
				filaProcessos.erase(filaProcessos.begin()+i);
				i--;
			}
		}
		sort(procProntos.begin(), procProntos.end(), comparacao);
		Processo p = procProntos[0];
		qProcesso--;
		procProntos.erase(procProntos.begin());

		tEspera += tExec - p.tChegada;
		tResposta += tExec - p.tChegada;
		tExec += p.tPico;
		tRetorno += tExec - p.tChegada;
	
	}

	float mediaRetorno = (tRetorno/tamFila);
	float mediaResposta = (tResposta/tamFila);
	float mediaEspera = (tEspera/tamFila);

	cout << "SJF " << mediaRetorno << " " << mediaResposta << " " << mediaEspera << endl; 

}

void RoundRobin(const vector <Processo> filaProcessos){ //FALHA DE SEGMENTAÇÃO
	float tRetorno = 0, tResposta = 0, tEspera = 0, tExec = 0;
	int tamFila = filaProcessos.size(); //tamanho do vetor
	int qProcesso = tamFila;
	bool voltaFila = false;

	vector <Processo> procProntos;
	vector <Processo> procNovos; //fila de novos processos
	Processo p;
	p.tPico = 0;
	p.tChegada = 0;

	for(int i = 0; i < filaProcessos.size(); i++){
		procNovos.push_back(filaProcessos[i]);;
	}

	while(qProcesso != 0){
		for(int i = 0; i < procProntos.size(); i++){
			if(procNovos[i].tChegada == tExec){
				procProntos.push_back(procNovos[i]);
				procNovos.erase(procNovos.begin()+i);
				i--;
				
			}else if(procNovos[i].tChegada < tExec){
				tEspera += tExec - procNovos[i].tChegada;
				procProntos.push_back(procNovos[i]);
				procNovos.erase(procNovos.begin()+i);
				i--;
			}
		}

		if(voltaFila){
			procProntos.push_back(p);
			voltaFila = false;
		}

		p = procProntos[0];
		procProntos.erase(procProntos.begin());

		if(p.tRestante == p.tPico){
			tResposta += tExec - p.tChegada;
		}

		tExec += QUANTUM;
		p.tRestante -= QUANTUM;
		tEspera += QUANTUM * procProntos.size();


		if(p.tRestante > 0){
			voltaFila = true;
		}

		if(p.tRestante <= 0){
			qProcesso--;
			tExec += p.tRestante;
			tEspera += (p.tRestante * int(procProntos.size()));
			tRetorno += tExec - p.tChegada;
		}

	}

	float mediaRetorno = (tRetorno/tamFila);
	float mediaResposta = (tResposta/tamFila);
	float mediaEspera = (tEspera/tamFila);

	cout << "RR " << mediaRetorno << " " << mediaResposta << " " << mediaEspera << endl; 

}


int main(){
	FILE *arq;
	vector <Processo> filaProcessos;
	int tamFila = filaProcessos.size();

	filaProcessos = LerArquivo(arq);
	FCFS(filaProcessos);
	SJF(filaProcessos);
	RoundRobin(filaProcessos);

	return 0;

}