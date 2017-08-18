/*

Universidade da Madeira
Faculdade de Ciências Exatas e da Engenharia

Estruturas de Dados e Algoritmos
Projecto Aeroporto EDA
2016/2017

*/

#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <conio.h>

using namespace std;

const char* nome_do_aeroporto = "aeroporto EDA";

/**********************************************************************************************************
* ficheiros
**********************************************************************************************************/

/**
 * estrutura que guarda uma referência para o ficheiro e a quantidade de linhas
 */
struct fich {
	char* 	nome_ficheiro	= new char[25];
	int 	max_linhas		= 0;
};

/**
 * criação de uma nova estrutura de ficheiro
 * @param nome_ficheiro - nome do ficheiro
 * @return referência para a estrutura criada
 */
struct fich* ficheiro_novo(char* nome_ficheiro) {
	struct fich* 	novo	= new fich;
	ifstream 		ficheiro(nome_ficheiro);
	int 			linhas	= 0;
	string 			linha;
	novo->nome_ficheiro = nome_ficheiro;
	if (ficheiro.is_open()) {
		while (getline(ficheiro, linha).good()) {
			linhas++;
		}
	}
	novo->max_linhas = linhas;
	return novo;
}

/**
 * escolhe uma linha aleatória no ficheiro
 * @param fich*
 * @return char* linha escolhida
 */
char* gera(struct fich* f) {
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

/**
 * estruturas com todos os ficheiros necessários à simulação
 */
struct fich* capacidade		= ficheiro_novo((char*)"capacidade.txt");
struct fich* destino		= ficheiro_novo((char*)"destino.txt");
struct fich* modelo			= ficheiro_novo((char*)"modelo.txt");
struct fich* nacionalidade	= ficheiro_novo((char*)"nacionalidade.txt");
struct fich* origem			= ficheiro_novo((char*)"origem.txt");
struct fich* primeiro_nome	= ficheiro_novo((char*)"primeiro_nome.txt");
struct fich* segundo_nome	= ficheiro_novo((char*)"segundo_nome.txt");
struct fich* voo			= ficheiro_novo((char*)"voo.txt");

/**********************************************************************************************************
* passageiros
**********************************************************************************************************/

/**
 * variável que se ocupa de incrementar o bilhete dos passageiros à medida que vão sendo gerados
 */
int bilhete_conta = 1;

/**
 * estrutura que representa cada passageiro
 */
struct passageiro {
	char* 				primeiro_nome;
	char* 				segundo_nome;
	char*				nacionalidade;
	int 				bilhete			= bilhete_conta++;
	struct passageiro*	seguinte		= NULL;
};

/**
 * mostra a informação guardada na estrutura passageiro
 * @param passageiro* referência para um passageiro
 */
void passageiro_mostra(struct passageiro* p) {
	cout << "\tbilhete: TK" << setfill('0') << setw(10) << p->bilhete << "\t"
		 << "nome: " << p->segundo_nome << " " << p->primeiro_nome << "\t"
		 << " nacionalidade: " << p->nacionalidade
		 << endl;
}

/**
 * mostra todos os passageiros numa lista
 * @param passageiro* referência para passageiro
 */
void passageiros_mostra(struct passageiro* lista_ref) {
	while (lista_ref != NULL) {
		passageiro_mostra(lista_ref);
		lista_ref = lista_ref->seguinte;
	}
}

/**
 * criação de passageiro
 * @param char* primeiro_nome - obtido aleatoriamente de um ficheiro
 * @param char*
 * @param char*
 * @return passageiro* - referência ao passageiro recém criado
 */
struct passageiro* passageiro_novo(char* primeiro_nome, char* segundo_nome, char* nacionalidade) {
	struct passageiro* passageiro_novo	= new passageiro;
	passageiro_novo->primeiro_nome		= primeiro_nome;
	passageiro_novo->segundo_nome		= segundo_nome;
	passageiro_novo->nacionalidade		= nacionalidade;
	return passageiro_novo;
}

/**
 * insere estrutura passageiro numa lista
 * @param passageiro* lista fornecida - irá receber o passageiro
 * @param passageiro* referência para o passageiro a inserir
 * @return
 */
struct passageiro* passageiro_insere(struct passageiro* lista_ref, struct passageiro* passageiro) {
	if (lista_ref == NULL) {
		lista_ref = passageiro;
	} else {
		struct passageiro* temp;
		for (temp = lista_ref; temp->seguinte != NULL; temp = temp->seguinte);
		temp->seguinte = passageiro;
	}
	return lista_ref;
}

/**
 * cria uma quantidade fornecida de passageiros
 * @param passageiro lista_ref lista de passageiros
 * @param int quantidade a gerar
 * @return
 */
struct passageiro* gera_passageiros(struct passageiro* lista_ref, int quantidade) {
	for (int i = 0; i < quantidade; i++) {
		lista_ref = passageiro_insere(lista_ref, passageiro_novo(gera(primeiro_nome), gera(segundo_nome), gera(nacionalidade)));
	}
	return lista_ref;
}

/**********************************************************************************************************
* avioes
**********************************************************************************************************/

/**
 * estrutura avião - tem uma referência para o avião seguinte na fila e contém uma fila de passageiros
 */
struct aviao {
	char* 				voo			= new char[25];
	char* 				modelo		= new char[25];
	char* 				origem		= new char[50];
	char* 				destino		= new char[50];
	int 				capacidade;
	struct passageiro*	passageiros = NULL;
	struct aviao*		seguinte	= NULL;
};

/**
 * criacao de aviao
 * @param char* voo
 * @param char* modelo
 * @param char* origem
 * @param int capacidade
 * @param char* destino - por defeito, como os aviões na sua maioria são gerados na aproximação, será o aeroporto da simulação
 * @return aviao* aviao recem criado
 */
struct aviao* aviao_novo(char* voo, char* modelo, char* origem, int capacidade, char* destino = (char*)nome_do_aeroporto) {
	aviao* aviao_novo		= new aviao;
	aviao_novo->voo			= voo;
	aviao_novo->modelo		= modelo;
	aviao_novo->origem		= origem;
	aviao_novo->destino		= destino;
	aviao_novo->capacidade	= capacidade;
	aviao_novo->passageiros = gera_passageiros(aviao_novo->passageiros, aviao_novo->capacidade);
	return aviao_novo;
}

/**
 * insere a estrutura aviaõ numa lista, fornecida
 * @param aviao* lista_ref
 * @param aviao* aviao_novo
 * @return aviao* referência para o primeiro elemento da fila fornecida
 */
struct aviao* aviao_insere(struct aviao* lista_ref, struct aviao* aviao_novo) {
	if (lista_ref == NULL) {
		lista_ref = aviao_novo;
	} else {
		struct aviao* temp;
		for (temp = lista_ref; temp->seguinte != NULL; temp = temp->seguinte);
		temp->seguinte = aviao_novo;
	}
	return lista_ref;
}

/**
 * mostra a informação do aviao
 * @param aviao* a
 */
void aviao_mostra(aviao* a) {
	cout << "voo: "			<< a->voo
		 << " modelo: "		<< a->modelo
		 << " origem: "		<< a->origem
		 << " destino: "	<< a->destino
		 << " capacidade: "	<< a->capacidade
		 << endl;
	if(a->capacidade > 0){
		passageiros_mostra(a->passageiros);
	}
	else {
		cout << "\tsem passageiros" << endl;
	}
	cout << endl << endl;
}

/**********************************************************************************************************
* simulacao
**********************************************************************************************************/

/**
 * definição das capacidades das diferentes zonas do aeroporto
 */
const int max_avioes_pista = 7;
const int max_avioes_aproximacao = 10;
const int max_avioes_descolagem = 5;
//const int max_ciclos_passageiros = 2;

//int sala_controlo = 1;

/**
 * estrutura que representa uma zona do aeroporto
 */
struct aeroporto_zona {
	char* 			nome;
	int 			vagas;
	int 			max;
	struct aviao*	avioes = NULL;
};
/*
struct sala_desembarque{
	struct passageiro* passageiros = NULL;
	char* voo;
	struct sala_desembarque* seguinte = NULL;
};
*/
/*
struct sala_desembarque* sala_desembarque_insere(struct sala_desembarque* lista_ref, struct passageiro* passageiros, char* voo) {
	struct sala_desembarque* nova = new sala_desembarque;
	nova->passageiros = passageiros;
	nova->voo = voo;
	if (lista_ref == NULL) {
		lista_ref = nova;
		cout << "aqui" << endl;
	}
	else {
		struct sala_desembarque* temp;
		for (temp = lista_ref; temp->seguinte != NULL; temp = temp->seguinte);
		temp->seguinte = nova;
	}
	return lista_ref;
}

struct sala_desembarque* sala_desembarque_insere(struct sala_desembarque* salas, struct passageiro* passageiros, char* voo) {
	int conta = 0;
	struct sala_desembarque* filtro = salas;
	if (filtro == NULL) {
		filtro = new sala_desembarque;
//		filtro = salas;
	} else {
		for (filtro = salas; filtro->seguinte != NULL; filtro = filtro->seguinte) {
			conta++;
		};
		filtro = new sala_desembarque;
	}
	filtro->passageiros = passageiros;
	return salas;
	/*
	struct sala_desembarque* filtro = salas;
	if (salas == NULL) {
		cout << "entra sempre aqui" << endl;
		salas = new sala_desembarque;
		filtro = salas;
	} else {
		cout << "ali" << endl;
		filtro = salas;
		while (filtro->seguinte != NULL) {
			cout << "volta do filtro" << endl;
			filtro = salas->seguinte;
			conta++;
		}
	}
	filtro->voo = voo;

	while (passageiros != NULL) {
		if (strcmp(passageiros->nacionalidade, "Portugal") == 0) {
			passageiro_insere(filtro->passageiros, passageiros);
			passageiros_mostra(filtro->passageiros);
		}
		passageiros = passageiros->seguinte;
	}
}	
*/

/**
 * criação de uma nova zona do aeroporto
 * @param int max_vagas - capacidade para aviões
 * @param
 * @return
 */
struct aeroporto_zona* aeroporto_zona_nova(int max_vagas, char* nome) {
	struct aeroporto_zona* novo = new aeroporto_zona;
	novo->nome	= nome;
	novo->vagas = novo->max = max_vagas;
	return novo;
}

/**
 * insere avião numa zona do aeroporto
 * @param aeroporto_zona* zona - zona a inserir
 * @param aviao* aviao - aviao a inserir na zona
 * @return aeroporto_zona - referência para o primeiro avião da lista fornecida
 */
struct aeroporto_zona* zona_insere(struct aeroporto_zona * zona, struct aviao* aviao) {
	if (zona->avioes == NULL) {
		zona->avioes = aviao;
	} else {
		struct aviao* temp;
		for (temp = zona->avioes; temp->seguinte != NULL; temp = temp->seguinte);
		temp->seguinte = aviao;
	}
	zona->vagas--;
	return zona;
}

/**
 * retira aviao de zona do aeroporto
 * @param aeroporto_zona* 
 * @return aviao* - o primeiro da fila
 */
struct aviao* zona_retira(struct aeroporto_zona* zona) {
	struct aviao* temp = zona->avioes;
	zona->avioes = zona->avioes->seguinte;
	temp->seguinte = NULL;
	zona->vagas++;
	return temp;
}

/**
 * ciclo - trata de toda a lógica da simulação
 * @param aeroporto_zona* aproximacao
 * @param aeroporto_zona* pista
 * @param aeroporto_zona* descolagem
 * @param aeroporto_zona* emergencia - activa a "aterragem" de  um aviao em emergencia
 */
void ciclo(struct aeroporto_zona* aproximacao, struct aeroporto_zona* pista, struct aeroporto_zona* descolagem, bool emergencia = false){//, struct passageiro* sala_desembarque, char* ultimo_voo, bool emergencia = false){
	if (descolagem->vagas == 0) {
		delete zona_retira(descolagem);
	}
	if (pista->vagas == 0) {
		struct aviao* aviao_descola	= zona_retira(pista);
		aviao_descola->passageiros	= gera_passageiros(aviao_descola->passageiros, aviao_descola->capacidade);
		aviao_descola->destino = gera(destino);
		aviao_descola->origem = (char*)nome_do_aeroporto;
		aviao_descola->voo = gera(voo);

		zona_insere(descolagem, aviao_descola);
	}
	if (aproximacao->vagas == 0 || emergencia) {
		struct aviao* aviao_aterra;
		if (emergencia) {
			aviao_aterra = aviao_novo("EMERGENCIA", gera(modelo), gera(origem), atoi(gera(capacidade)));
		} else {
			aviao_aterra = zona_retira(aproximacao);
		}
		aviao_aterra->passageiros = NULL;

		/*	cout << "aviao aterrou" << endl;
		passageiros_mostra(aviao_aterra->passageiros);
		sala_desembarque = aviao_aterra->passageiros;
		ultimo_voo = aviao_aterra->voo;
		*/
		zona_insere(pista, aviao_aterra);
	}
	if (aproximacao->vagas > 0 && !emergencia) {
		zona_insere(aproximacao, aviao_novo(gera(voo), gera(modelo), gera(origem), stoi(gera(capacidade))));
	}
}

/**
 * mostra a informacao (aviões, etc) contida numa estrutura que representa uma zona do aeroporto
 * @param aeroporto_zona*
 */
void zona_mostra(aeroporto_zona* zona) {
	cout << endl << "->" << zona->nome << ": " << endl;
	if(zona->max - zona->vagas > 0){
		struct aviao* temp = zona->avioes;
		while (temp != NULL) {
			aviao_mostra(temp);
			temp = temp->seguinte;
		}
	}
	else {
		cout << "\tsem avioes" << endl;
	}
}
/*
void sala_desembarque_mostra(struct passageiro* sala_desembarque, char* voo){
	cout << endl << "-> zona de desembarque" << endl;
	
	if (sala_desembarque == NULL) {
		cout << "\tsem passageiros" << endl;
	} else {
		passageiros_mostra(sala_desembarque);
	}
	cout << endl;
}
*/

/**
 * mostra todas as zonas do aeroporto
 * @param aeroporto_zona* aproximacao
 * @param aeroporto_zona* pista
 * @param aeroporto_zona* aterragem
 */
void aeroporto_mostra(struct aeroporto_zona* aproximacao, struct aeroporto_zona* pista, struct aeroporto_zona* descolagem){// , struct passageiro* sala_desembarque, char* ultimo_voo){
	cout << endl << endl << "*** ciclo ***" << endl;
	zona_mostra(descolagem);
	zona_mostra(pista);
//	sala_desembarque_mostra(sala_desembarque, ultimo_voo);
	zona_mostra(aproximacao);
}

/**********************************************************************************************************
* auxiliares
**********************************************************************************************************/

/**
 * componente presente no desenho da moldura do menu
 * @param int qtd - quantos caracteres para fazer a "linha horizontal"
 * @param topo
 */
void desenha_linha_horizontal(int qtd, bool topo = false) {
	if (topo) {
		cout << char(201);
	} else {
		cout << char(200);
	}
	for (int i = 0; i < qtd; i++) {
		cout << char(205);
	}
	if (topo) {
		cout << char(187);
	} else {
		cout << char(188);
	}
	cout << endl;
}

/**
 * pesquisa por um avião nas diferentes zonas do aeroporto - mostra informações e passageiros, se houver (na pista, os aviões estão vazios)
 * @param string pesquisa - termo de pesquisa por voo ou modelo de avião
 * @param aeroporto_zona* aprox
 * @param aeroporto_zona* pista
 * @param aeroporto_zona* descol
 */
void pesquisa_aviao(string pesquisa, struct aeroporto_zona* aprox, struct aeroporto_zona* pista, struct aeroporto_zona* descol) {
	struct aviao* temp = aprox->avioes;
	for (temp = aprox->avioes; temp != NULL; temp = temp->seguinte) {
		if (temp != NULL) {
			if (pesquisa.compare((string)temp->voo) == 0 || pesquisa.compare((string)temp->modelo) == 0) {
				cout << "AVIAO EM APROXIMACAO:" << endl;
				aviao_mostra(temp);
			};
		}
	}
	for (temp = pista->avioes; temp != NULL; temp = temp->seguinte) {
		if (temp != NULL) {
			if (pesquisa.compare((string)temp->voo) == 0 || pesquisa.compare((string)temp->modelo) == 0) {
				cout << "AVIAO NA PISTA:" << endl;
				aviao_mostra(temp);
			};
		}
	}
	for (temp = descol->avioes; temp != NULL; temp = temp->seguinte) {
		if (temp != NULL) {
			if (pesquisa.compare((string)temp->voo) == 0 || pesquisa.compare((string)temp->modelo) == 0) {
				cout << "AVIAO JA DESCOLOU:" << endl;
				aviao_mostra(temp);
			};
		}
	}

}
/**********************************************************************************************************
* principal
**********************************************************************************************************/
int main(int argc, char **argv) {
	int i = 0;
	if (argc > 1) {
		ifstream ficheiro_leitura(argv[1]);
//		cout << getline() << endl;
	}
	srand(time(0));
	// simulacao
	struct aeroporto_zona*	aproximacao			= aeroporto_zona_nova(max_avioes_aproximacao,	(char*)"avioes em aproximacao");
	struct aeroporto_zona*	pista				= aeroporto_zona_nova(max_avioes_pista,			(char*)"avioes na pista");
	struct aeroporto_zona*	descolagem			= aeroporto_zona_nova(max_avioes_descolagem,	(char*)"avioes que descolaram");
//	struct passageiro*		sala_desembarque	= NULL;
//	char*					ultimo_voo			= NULL;

	cout << "('O' PARA VER AS OPCOES)" << endl;
	
	unsigned char input;
	do {
		input = _getch();
		if (input == 'c' || input == 'C') {
			ciclo(aproximacao, pista, descolagem);// , sala_desembarque, ultimo_voo);// , ciclos_de_passageiros);
			aeroporto_mostra(aproximacao, pista, descolagem);// , sala_desembarque, ultimo_voo);// , ciclos_de_passageiros);

		} else if (input == 'g' || input == 'G') {
			cout << "grava" << endl;
			ofstream escrita;
			escrita.open("g.txt");
			/**for (i = 0; i < aproximacao->max; i++) {
				escrita << aproximacao->avioes[i]->capacidade << aproximacao->avioes[i]->destino << aproximacao->avioes[i]->modelo;
			}**/
			escrita.close();
		} else if (input == 'e' || input == 'E') {
			ciclo(aproximacao, pista, descolagem, true);
			aeroporto_mostra(aproximacao, pista, descolagem);
		} else if (input == 'a' || input == 'A') {
			cout << "nome ou voo do aviao: ";
			string aviao;
			cin >> aviao;
			pesquisa_aviao(aviao, aproximacao, pista, descolagem);
		} else if (input == 'p' || input == 'P') {
			cout << "pesquisa passageiro" << endl;
		} else if (input == 'o' || input == 'O') {
			desenha_linha_horizontal(17, true);
			cout << char(186) << " (O)OPCOES       " << char(186) << endl
				 << char(186) << " (C)ICLO         " << char(186) << endl
				 << char(186) << " (E)MERGENCIA    " << char(186) << endl
				 << char(186) << " (A)VIAO         " << char(186) << endl
				 << char(186) << " (P)ASSAGEIRO  X " << char(186) << endl
				 << char(186) << " (G)RAVA       X " << char(186) << endl
				 << char(186) << " (S)AIR          " << char(186) << endl;
			desenha_linha_horizontal(17);
		}
	} while (input != 's' && input != 'S');
	return 0;
}