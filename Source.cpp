#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <conio.h>
#include "aeroporto.h"

using namespace std;

/**********************************************************************************************************
* ficheiros
**********************************************************************************************************/

fich* ficheiro_novo(char* nome_ficheiro) {
	fich* 		novo = new fich;
	ifstream 	ficheiro(nome_ficheiro);
	int 		linhas = 0;
	string 		linha;
	novo->nome_ficheiro = nome_ficheiro;
	if (ficheiro.is_open()) {
		while (getline(ficheiro, linha).good()) {
			linhas++;
		}
	}
	novo->max_linhas = linhas;
	return novo;
}

char* gera(fich* f) {
	int 		aleatorio = 1 + rand() % (f->max_linhas - 1);
	ifstream 	ficheiro(f->nome_ficheiro);
	char* 		linha = new char[50];
	int 		linhas = 0;
	string		temp;
	if (ficheiro.is_open()) {
		while (getline(ficheiro, temp).good()) {
			if (aleatorio == linhas++) {
				strcpy_s(linha, 50, temp.c_str());
				return (char*)linha;
			}
		}
	}
	return (char*)"";
}

// ficheiros
fich* capacidade = ficheiro_novo((char*)"capacidade.txt");
fich* destino = ficheiro_novo((char*)"destino.txt");
fich* modelo = ficheiro_novo((char*)"modelo.txt");
fich* nacionalidade = ficheiro_novo((char*)"nacionalidade.txt");
fich* origem = ficheiro_novo((char*)"origem.txt");
fich* primeiro_nome = ficheiro_novo((char*)"primeiro_nome.txt");
fich* segundo_nome = ficheiro_novo((char*)"segundo_nome.txt");
fich* voo = ficheiro_novo((char*)"voo.txt");

/**********************************************************************************************************
* passageiros
**********************************************************************************************************/

int bilhete_conta = 0;

void passageiro_mostra(passageiro* p) {
	cout << "\tbilhete: TK" << setfill('0') << setw(10) << p->bilhete << "\t"
		<< "nome: " << p->segundo_nome << " " << p->primeiro_nome << "\t"
		<< " nacionalidade: " << p->nacionalidade
		<< endl;
}

passageiro* passageiro_novo(char* primeiro_nome, char* segundo_nome, char* nacionalidade, int bilhete = bilhete_conta++) {
	passageiro* novo	= new passageiro;
	novo->primeiro_nome = primeiro_nome;
	novo->segundo_nome	= segundo_nome;
	novo->nacionalidade = nacionalidade;
	novo->bilhete		= bilhete;
	return novo;
}

/**********************************************************************************************************
* avioes
**********************************************************************************************************/

aviao* aviao_novo(char* voo, char* modelo, char* origem, int capacidade, char* destino = (char*)nome_do_aeroporto) {
	aviao* novo			= new aviao;
	novo->voo			= voo;
	novo->modelo		= modelo;
	novo->origem		= origem;
	novo->destino		= destino;
	novo->capacidade	= capacidade;
	novo->passageiros	= new passageiro*[capacidade];
	for(int i = 0; i < novo->capacidade; i++)
		novo->passageiros[i] = passageiro_novo( gera(primeiro_nome), gera(segundo_nome), gera(nacionalidade) );
	return novo;
}

void aviao_mostra(aviao* a) {
	cout << "voo: " << a->voo
		<< " modelo: " << a->modelo
		<< " origem: " << a->origem
		<< " destino: " << a->destino
		<< " capacidade: " << a->capacidade
		<< endl;
	if (a->capacidade > 0 && a->passageiros != NULL) {
		for (int i = 0; i < a->capacidade; i++) {
			passageiro_mostra( a->passageiros[i] );
		}
	}
	else {
		cout << "\tsem passageiros" << endl;
	}
	cout << endl << endl;
}

/**********************************************************************************************************
* simulacao
**********************************************************************************************************/

aeroporto_zona* aeroporto_zona_nova(int max_vagas, char* nome) {
	aeroporto_zona* novo = new aeroporto_zona;
	novo->nome = nome;
	novo->avioes = new aviao*[max_vagas];
	novo->vagas = novo->max = max_vagas;
	return novo;
}

void ciclo(aeroporto_zona* aproximacao, aeroporto_zona* pista, aeroporto_zona* descolagem){//, passageiros_em_terra* ciclos_de_passageiros) {
	int i; // iterator
	if (descolagem->vagas == 0) {
		descolagem->avioes[0] = NULL;
		delete descolagem->avioes[0];
		descolagem->vagas++;
		for (i = 0; i < descolagem->max - descolagem->vagas; i++) {
			descolagem->avioes[i] = descolagem->avioes[i + 1];
		}
	}
	if (pista->vagas == 0) {
		pista->avioes[0]->destino = gera(destino);
		pista->avioes[0]->origem = (char*)nome_do_aeroporto;
		// gerou um conjunto novo de passageiros
		// passageiros_gera(pista->avioes[0]->passageiros, pista->avioes[0]->capacidade);
		pista->avioes[0]->passageiros = new passageiro*[pista->avioes[0]->capacidade];
		for (i = 0; i < pista->avioes[0]->capacidade; i++) {
			pista->avioes[0]->passageiros[i] = passageiro_novo(gera(primeiro_nome), gera(segundo_nome), gera(nacionalidade));
		}
		descolagem->avioes[descolagem->max - descolagem->vagas] = pista->avioes[0];
		descolagem->vagas--;
		pista->vagas++;
		for (i = 0; i < pista->max - pista->vagas; i++) {
			pista->avioes[i] = pista->avioes[i + 1];
		}
	}
	if (aproximacao->vagas == 0) {
		aproximacao->avioes[0]->passageiros = NULL;
		pista->avioes[pista->max - pista->vagas] = aproximacao->avioes[0];
		pista->vagas--;
		for (i = 0; i< aproximacao->max - aproximacao->vagas; i++){
			aproximacao->avioes[i] = aproximacao->avioes[i + 1];
		}
		aproximacao->vagas++;
	}
	if (aproximacao->vagas > 0) {
		aproximacao->avioes[aproximacao->max - aproximacao->vagas] = aviao_novo( gera(voo), gera(modelo), gera(origem), stoi(gera(capacidade)) );
		aproximacao->vagas--;
	}
}

void zona_mostra(aeroporto_zona* zona) {
	cout << endl << "->" << zona->nome << ": " << endl;
	int i;
	if (zona->vagas < zona->max) {
		for (i = 0; i < zona->max - zona->vagas; i++) {
			aviao_mostra(zona->avioes[i]);
		}
	}
	else {
		cout << "\tsem avioes" << endl;
	}
}

void aeroporto_mostra(aeroporto_zona* aproximacao, aeroporto_zona* pista, aeroporto_zona* descolagem){//, passageiros_em_terra* ciclos_de_passageiros) {
	cout << endl << endl << "*** ciclo ***" << endl;
	zona_mostra(descolagem);
	zona_mostra(pista);
	zona_mostra(aproximacao);
	//passageiros_em_terra_mostra(ciclos_de_passageiros);
}

void emergencia(aeroporto_zona* aproximacao, aeroporto_zona* pista, aeroporto_zona* descolagem) {

	char voo_emergencia;
	char voo_descolar;

	cout << "Qual é o número do voo que precisa de aterrar com emergência?" << endl;
	cin >> voo_emergencia;

	if (pista->vagas == 0) {

		cout << "A pista não tem espaço para o voo emergência.\n" <<
			"Qual é o número do voo que quer retirar da pista?";

		cin >> voo_descolar;

		// o ideal seria criar uma função para cada zona que executa eventos no ciclo

		if (descolagem->vagas == 0) {

			descolagem->avioes[0] = NULL;
			delete descolagem->avioes[0];

			descolagem->vagas++;

			for (int i = 0; i < descolagem->max - descolagem->vagas; i++) {
				descolagem->avioes[i] = descolagem->avioes[i + 1];
			}

		}



	}



}

void ler_ficheiro(aeroporto_zona* aproximacao, aeroporto_zona* pista, aeroporto_zona* descolagem, char **argv) {

	// a leitura do ficheiro é feita APÓS a criação de structs nas variáveis

	ifstream ficheiro_leitura;
	ficheiro_leitura.open(argv[1]);

	if (!ficheiro_leitura.fail()) {

		for (int i = 0; i < (aproximacao->max - aproximacao->vagas); i++) {

			ficheiro_leitura.ignore(20, 'voo');
			ficheiro_leitura.seekg(2, ficheiro_leitura.beg);

		}

		// max_avioes_aproximacao tem de ser alterado
		//for (int i = 0; i < max_avioes_aproximacao; i++) {

		//	getline(ficheiro_leitura)

		//}

	}

}

/**********************************************************************************************************
* principal
**********************************************************************************************************/
int main(int argc, char **argv) {
	int i = 0;

	/*
		Leitura do ficheiro de texto previamente gravado com dados do aeroporto (por agora,
		é o ficheiro de texto "g.txt")
	*/

	//if (argc > 1) {

	//	ifstream ficheiro_leitura(argv[1]);
	//	// cout << getline() << endl;
	//}


	srand(time(0));

	// simulacao
	aeroporto_zona* aproximacao = aeroporto_zona_nova(max_avioes_aproximacao, (char*)"avioes em aproximacao");
	aeroporto_zona* pista = aeroporto_zona_nova(max_avioes_pista, (char*)"avioes na pista");
	aeroporto_zona* descolagem = aeroporto_zona_nova(max_avioes_descolagem, (char*)"avioes para descolar");

	//passageiros_em_terra* ciclos_de_passageiros = passageiros_em_terra_nova(max_ciclos_passageiros);

	if (argc > 1) {

		ler_ficheiro(aproximacao, pista, descolagem, argv);

	}

	// interaccao
	unsigned char input;
	do {

		cout << "(c)iclo (s)air (g)ravar (e)mergencia" << endl;

		input = _getch();
		if (input == 'c') {
			ciclo(aproximacao, pista, descolagem);// , ciclos_de_passageiros);
			aeroporto_mostra(aproximacao, pista, descolagem);// , ciclos_de_passageiros);
		}

		if (input == 'e') {

			emergencia();

		}

		if (input == 'g') {
			ofstream escrita;
			escrita.open("g.txt");

			// escrita << "aproximacao: ";

			for (i = 0; i < (aproximacao->max - aproximacao->vagas); i++) {

				// escrita << aproximacao->avioes[i]->capacidade << ";" << aproximacao->avioes[i]->destino << ";" << aproximacao->avioes[i]->modelo;
				// escrita << "{" << aproximacao->avioes[i]->modelo << "," << aproximacao->avioes[i]->destino << "," << aproximacao->avioes[i]->capacidade << "},";

				// --- representação mais pipi da escrita no ficheiro de texto --- //

				/* escrita << "Avião " << aproximacao->avioes[i] << ":\n\n" <<
					"Modelo: " << aproximacao->avioes[i]->modelo << "\n" <<
					"Destino: " << aproximacao->avioes[i]->destino << "\n" <<
					"Capacidade: " << aproximacao->avioes[i]->capacidade << "\n" << endl;
				*/

				// --------------------------------------------------------------- //

				escrita << "{\n"
					"\taproximacao: " << i << " {\n" << 
					"\t\tvoo: " << aproximacao->avioes[i]->voo << ",\n" <<
					"\t\tmodelo: " << aproximacao->avioes[i]->modelo << ",\n" <<
					"\t\torigem: "  << aproximacao->avioes[i]->origem << ",\n" <<
					"\t\tdestino: " << aproximacao->avioes[i]->destino << ",\n" <<
					"\t\tcapacidade: " << aproximacao->avioes[i]->capacidade << ",\n";

					for (int j = 0; j < aproximacao->avioes[i]->capacidade; j++) {
						escrita << "\t\tpassageiro: " << j << " {\n" << 
						"\t\t\tbilhete: TK" << setfill('0') << setw(10) << aproximacao->avioes[i]->passageiros[j]->bilhete << ",\n" <<
						"\t\t\tnacionalidade: " << aproximacao->avioes[i]->passageiros[j]->nacionalidade << ",\n" <<
						"\t\t\tprimeiro_nome: " << aproximacao->avioes[i]->passageiros[j]->primeiro_nome << ",\n" <<
						"\t\t\tsegundo_nome: " << aproximacao->avioes[i]->passageiros[j]->segundo_nome << ",\n";

						escrita << "\t\t}\n";

					}

					escrita << "}\n";

			}

			escrita << "\n";
			escrita << "pista: ";

			for (i = 0; i < (pista->max - pista->vagas); i++) {

				escrita << "{" << pista->avioes[i]->voo << ";" <<
						pista->avioes[i]->modelo << ";" <<
						pista->avioes[i]->origem << ";" <<
						pista->avioes[i]->destino << ";" <<
						// pista->avioes[i]->capacidade << ";" <<

						pista->avioes[i]->capacidade << "}, ";

						// pista->avioes[i]->passageiros[i] << "}, ";

			}

			escrita << "\n";
			escrita << "descolagem: ";

			for (i = 0; i < (descolagem->max - descolagem->vagas); i++) {

				escrita << "{" << descolagem->avioes[i]->modelo << ";" << descolagem->avioes[i]->destino << ";" << descolagem->avioes[i]->capacidade << "},";

			}

			escrita << "\n";

			escrita.close();

			cout << "Ficheiro gravado com sucesso." << endl;

		}
	} while (input != 's' && input != 'S');
	return 0;
}
