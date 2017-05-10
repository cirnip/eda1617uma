struct fich {
	char* 	nome_ficheiro = new char[25];
	int 	max_linhas = 0;
};

struct passageiro {
	char* 	primeiro_nome = new char[25];
	char* 	segundo_nome = new char[25];
	char*	nacionalidade = new char[50];
	int 	bilhete;
};

// avião

struct aviao {
	char* 		voo = new char[25];
	char* 		modelo = new char[25];
	char* 		origem = new char[50];
	char* 		destino = new char[50];
	int 		capacidade;
	passageiro** passageiros;
};

// simulação

const char* nome_do_aeroporto = "Aeroporto EDA";

const int max_avioes_pista = 2;//7;
const int max_avioes_aproximacao = 2;//10;
const int max_avioes_descolagem = 2;//5;
const int max_ciclos_passageiros = 2;

struct aeroporto_zona {
	char* 	nome;
	aviao** avioes;
	int 	vagas;
	int 	max;
};