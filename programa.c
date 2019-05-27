#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct ma{
	double *m;
	int l;
	int c;
	int numt;
    };

double *inv;
int aux1 = 0;

void matriz(struct ma *m, char arq[100]){//nessa função faz a inserção de dados na matriz inicial
    	int i,j;
    	FILE *matriz_real;
    	matriz_real = fopen(arq, "r");//abertura do arquivo

    	for(i = 0; i < m->l; i++){
		for(j = 0;j < m->c; j++){
			fscanf(matriz_real,"%lf",&m->m[(m->c * i) + j]);
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

       	for(i = 0; i < m->c; i++){
        	k = m->c - 1 - aux;
		for(j = aux; j < m->l; j = j + m->numt){
           		inv[(m->c * i) + j] = m->m[(m->c * k) + a];
            		k = k - m->numt;
        	  }
		a++;
    	  }
	pthread_exit(NULL);
}

void imprimir(double *inv, int l, int c, char arq[100]){
	int i, j;
	FILE *matriz;
	matriz = fopen(arq,"w");
	for(i=0;i<l;i++){
		for(j=0;j<c;j++)
			fprintf(matriz,"%lf\t",inv[(c * i) + j]);
		fprintf(matriz,"\n");	
	}
	fclose(matriz);
}

/*int apagar(int **n,int **m, int l, int c){
    	int i,j;
    	for(i = 0;i < l;i++){
        	for(j = 0;j < c;j++){
            		free(n[j][i]);
            		free(m[i][j]);
        }
    }
    return 0;
}*/

void main(){

    	struct ma *m = (struct ma*) malloc(sizeof(struct ma));
    	char arq_e[101], arq_s[101];
   
    	printf("Informe os valores das linhas e colunas:\n");
    	scanf("%d", &m->l);
    	scanf("%d", &m->c);
    	printf("Informe o numero de threads:\n");
    	scanf("%d", &m->numt);
    	getchar();
    	printf("Digite o nome do arquivo de entrada: (.txt)\n");
    	fgets(arq_e, 100,stdin);
    	arq_e[strcspn(arq_e, "\n")]=0;
	printf("Digite o nome do arquivo de saida: (.txt)\n");
    	fgets(arq_s, 100,stdin);
    	arq_s[strcspn(arq_s, "\n")]=0;
    	
   
    	int i, j;
	
    	inv = (double*) malloc(m->l * m->c * sizeof(double*));
	
	m->m = (double*) malloc(m->l * m->c * sizeof(double*));
	
    	pthread_t vetT[m->numt];

    	matriz(m, arq_e);
    
    	for(i = 0; i < m->numt; i++){
		pthread_create(&vetT[i], NULL, inverter, (void *)m);
	  }

    	for(i = 0; i < m->numt; i++){
		pthread_join(vetT[i], NULL);
	  }

	imprimir(inv, m->l, m->c, arq_s);

    	/*for(i = 0; i < m->l; i++){
        	for(j = 0; j < m->c; j++)
            		printf("%.1lf\t",m->m[(m->c * i) + j]);
        	printf("\n");
    	}
	printf("\n");
    	for(i = 0; i < m->l; i++){
        	for(j = 0; j < m->c; j++)
            		printf("%.1lf\t",inv[(m->c * i) + j]);
        	printf("\n");
    	}*/
    //apagar(inv,m->m,m->l,m->c);
}
