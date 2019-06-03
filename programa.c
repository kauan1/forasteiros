#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

struct ma{//Estrutura para armazenar todos os dados necessarios para o giro de 90° e envio para a função pthread_create(que só aceita um parametro de dados)
	double *m;//matriz inicial
	int l;//numero de linhas
	int c;//numero de colunas
	int numt;//numero de threads
    };

double *inv;//Matriz que recebe as inversões da matriz inicial
int aux1 = 0;//auxiliar para indentificador de threads

void matriz(struct ma *m, char arq[100]){//nessa função faz a inserção de dados na matriz inicial
    	int i,j;
    	FILE *matriz_real;//cria um ponteiro de arquivo para abrir arquivos
    	matriz_real = fopen(arq, "r");//abertura do arquivo para leitura

    	for(i = 0; i < m->l; i++){
		for(j = 0;j < m->c; j++){
			fscanf(matriz_real,"%lf",&m->m[(m->c * i) + j]);////matriz inicial recebendo os dados
		    }
    }

    	fclose(matriz_real);//fechamento do arquivo
}

void *inverter(void *info){//nessa função se gira 90º a matriz inicial	
	int i, j, k, aux, a = 0;
	struct ma *m;
	m = (struct ma*)info;
    	aux = aux1;  	
	aux1++;
	
       	for(i = 0; i < m->c; i++){//'for' para explorar as linhas
        	k = m->c - 1 - aux;
		for(j = aux; j < m->l; j = j + m->numt){//'for' para explorar as colunas
		/*'j' recebe aux que seria o identificador da thread, para nem uma thread trabalhar no mesmo lugar da outra*/
           		inv[(m->c * i) + j] = m->m[(m->c * k) + a];
            		k = k - m->numt;//k é a variável 'linha' para o giro 
        	  }
		a++;//a é a variável 'coluna' para o giro
    	  }
	pthread_exit(NULL);//sai da thread
}

void imprimir(double *inv, int l, int c, char arq[100]){//função para salva inversão em um arquivo
	int i, j;
	FILE *matriz;//ponteiro de arquivo
	matriz = fopen(arq,"w");//abertura do arquivo para escrita
	for(i=0; i < c; i++){
		for(j=0; j < l; j++)
			fprintf(matriz,"%lf\t",inv[(c * i) + j]);//salvando dados nas posições
		fprintf(matriz,"\n");	
	}
	fclose(matriz);//fechamento do arquivo
}

void apagar(double *n,double *m){//Libera a memoria utilizada nas alocações
       	free(n);
       	free(m);
}

void main(int argc, char *argv[]){//Função main, onde todas as funções de cima são chamadas estrategicamente

    	struct ma *m = (struct ma*) malloc(sizeof(struct ma));//Objeto da struct ma
    	char *arq_e, *arq_s;//strings que receberam o nome dos arquivos a serem usado e criado
   	clock_t tempoInicial;//contadores de tempo
   	clock_t tempoFinal;
	double tempoExecucao;
   	//as variáveis recebem os valores
    	m->l = atof(argv[1]);
    	m->c = atof(argv[2]);
    	m->numt = atof(argv[3]);
	arq_e = (argv[4]);
    	arq_e[strcspn(arq_e, "\n")] = 0;
	arq_s = (argv[5]);
    	arq_s[strcspn(arq_s, "\n")]=0;
   
    	int i;
	
    	inv = (double*) malloc(m->l * m->c * sizeof(double*));//alocação de memória para a matriz que receberá os dados 90° girados 
	
	m->m = (double*) malloc(m->l * m->c * sizeof(double*));//alocação de memória para a matriz inicial
	
    	pthread_t vetT[m->numt];//vetor para receber o id das threads

    	matriz(m, arq_e);//chama a função para inicia a matriz inicial

    	tempoInicial = clock();//inicia a contagem de tempo de funcionamento das threads
    	for(i = 0; i < m->numt; i++){//cria a quantida de threads que o usuário deseja
		pthread_create(&vetT[i], NULL, inverter, (void *)m);
	  }

    	for(i = 0; i < m->numt; i++){//libera as thr
		pthread_join(vetT[i], NULL);
	  }
	tempoFinal = clock();//finaliza a contagem de tempo de funcionamento das threads

	tempoExecucao = (tempoFinal - tempoInicial) * 1000.0 / CLOCKS_PER_SEC;
	printf("Tempo para a conclusão da inversão com %d threads é de %lf\n", m->numt, tempoExecucao);

	imprimir(inv, m->l, m->c, arq_s);

    	apagar(inv,m->m);
}
