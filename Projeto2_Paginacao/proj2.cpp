#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>

using namespace std;

//vector <int> listaPagina;
int n_quadros; 

bool BuscaMemo(int *ar, int pagina){
	for(int i = 0; i < n_quadros; i++){
		if(pagina == ar[i]){
			return true;
		}
	}
	return false;
}

int BuscaMemo1(vector <int> ar, int pagina){ //usado para o OTM
	for(int i = 0; i < n_quadros; i++){
		if(pagina == ar[i]){
			return i;
		}
	}
	return -1; //se retornar -1 significa que a pagina nao esta na memoria
}


vector <int> LerArquivo(FILE *arq){
	arq = fopen("entrada.txt", "rt");
	int pAtual;

	vector<int> p;

	while(!feof(arq)){
		fscanf(arq, "%d", &pAtual);

		if(feof(arq)) 
			break;

		p.push_back(pAtual);
	}

	fclose(arq);

	n_quadros = p[0];
	p.erase(p.begin());


	return p;

}

void FIFO(vector<int> listaPagina){
	int faltaPag = 0;
	int nPagina = listaPagina.size();
	int memo[n_quadros] = {};
	int contador = 0;
	int z = 0;

	if(nPagina == 0){
		cout << "Nao existem paginas" << endl;
	}

	for(int i = 0; i < nPagina; i++){
		if(!BuscaMemo(memo, listaPagina[i])){
			if(contador == n_quadros){
				contador = 0;
			}
			memo[contador] = listaPagina[i];
			faltaPag++;
			contador++; 
		}	
	}
	cout << "FIFO" << " " << faltaPag << endl;

}

void OTM(vector <int> listaPagina){
	int faltaPag = 0;
	int nPagina = listaPagina.size(); //tamanho da lista
	int memo[n_quadros] = {}; //armazena os valores que estao nos quadros
	int contador = 0;
	int i = 0;

	if(nPagina == 0){
		cout << "Nao existem paginas" << endl;
	}

	for(i = 0; i < nPagina; i++){
		if(faltaPag < n_quadros && !BuscaMemo(memo, listaPagina[i])){
			memo[faltaPag] = listaPagina[i];
			faltaPag++;
			continue;
		}else if(!BuscaMemo(memo, listaPagina[i])){
			vector <int> aux;

			for(int k = 0; k < n_quadros; k++){
				aux.push_back(memo[k]);
			}

			for(int j = i; j < nPagina; j++){
				int pos = BuscaMemo1(aux, listaPagina[j]);
				if(pos >= 0 && aux.size() > 1){
					aux.erase(aux.begin()+pos);
				}
			}

			int pagRemoved = aux[0];
			for(int l = 0; l < n_quadros; l++){
				if(memo[l] == pagRemoved){
					memo[l] = listaPagina[i];
				}
			}
			faltaPag++;
		}
	}
	
	cout << "OTM" << " " << faltaPag << endl;

}

void LRU(vector <int> listaPagina){
	int faltaPag = 0;
	int nPagina = listaPagina.size();
	int memo[n_quadros] = {};
	int contador = 0;
	int i;

	if(nPagina == 0){
		cout << "Nao existem paginas" << endl;
	}

	for(i = 0; i < nPagina; i++){
		if(faltaPag < n_quadros && !BuscaMemo(memo, listaPagina[i])){
			memo[faltaPag] = listaPagina[i];
			faltaPag++;
			continue;
		}else if(!BuscaMemo(memo, listaPagina[i])){
			vector <int> aux;

			for(int k = 0; k < n_quadros; k++){
				aux.push_back(memo[k]);
			}

			for(int j = i; j > 0; j--){
				int pos = BuscaMemo1(aux, listaPagina[j]);
				if(pos >= 0 && aux.size() > 1){
					aux.erase(aux.begin()+pos);
				}
			}

			int pagRemoved = aux[0];
			for(int l = 0; l < n_quadros; l++){
				if(memo[l] == pagRemoved){
					memo[l] = listaPagina[i];
				}
			}
			faltaPag++;
		}
	}

	cout << "LRU" << " " << faltaPag << endl;

}



int main(){
	FILE *arq;

	vector <int> AlgoritmoFIFO;
	vector <int> AlgoritmoOtimo;
	vector <int> AlgoritmoLRU;
	

	AlgoritmoFIFO = LerArquivo(arq);
	AlgoritmoOtimo = LerArquivo(arq);
	AlgoritmoLRU = LerArquivo(arq);
	

	FIFO(AlgoritmoFIFO);
	OTM(AlgoritmoOtimo);
	LRU(AlgoritmoLRU);

	return 0;
	
}