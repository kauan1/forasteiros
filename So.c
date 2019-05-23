#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct ma{
	double **m;
	int l;
	int c;
	int numt;
	int i;
    };

double **inv;

void **matriz(double **m,int l, int c, char arq[100]){//nessa função faz a inserção de dados na matriz
    	int i,j;
    	FILE *matriz_real;

    	matriz_real = fopen(arq, "r");//abertura do arquivo
 
    	m = (double**) malloc(l*sizeof(double*));
    	for(i = 0; i < c; i++)
       		m[i] = (double*) malloc(c*sizeof(double));

    	for(i = 0; i < l; i++){
		for(j = 0;j < c; j++){
			fscanf(matriz_real,"%lf",&m[i][j]);
		}
	}
  	fclose(matriz_real);
}

void *inverter(void *m){//nessa função se gira 90º a mtriz 
    	int aux = ((struct ma*)m)->i;
    	printf("oi %d\n", aux);
   	int i, j, k, a = 0;
    //if(((struct ma*)m)->l >= ((struct ma*)m)->c){
       	for(i = 0; i < ((struct ma*)m)->c; i++){
		//printf("oi %d\n", aux);
        	k = ((struct ma*)m)->l - 1 - aux;
        	for(j = aux; j < ((struct ma*)m)->l; j = j + ((struct ma*)m)->numt){
			//printf("%lf", ((struct ma*)m)->m[i][j]);//erro falha de segmentação(imagem do nucleo gravada)
           	 	//inv[i][j] = ((struct ma*)m)->m[a][k];//erro falha de segmentação(imagem do nucleo gravada)
		 	printf("%d %d ",a,k);
            	k = k - ((struct ma*)m)->numt;
        	}
        a++;
    	}
     /*}
     else{
        for(i = 0;i < ((struct ma*)m)->c; i = i + ((struct ma*)m)->numt){
        	k = ((struct ma*)m)->l-1;
        	for(j = ((struct ma*)m)->numt; j < ((struct ma*)m)->l; j = j + ((struct ma*)m)->numt){
           	  inv[i][j] = ((struct ma*)m)->m[k][a];
		  printf("%d %d %f",k,a,inv[k][a]);
            	  k = k - ((struct ma*)m)->numt;
        	}
	a = a + ((struct ma*)m)->numt;
     	}
     }*/
	pthread_exit(NULL);
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

    	struct ma *m;
    	char arq_e[101];
   
    	printf("Informe os valores das linhas e colunas:\n");
    	scanf("%d", &m->l);
    	scanf("%d", &m->c);
    	m->numt = 2;
    	printf("Informe o numero de threads:\n");
    	scanf("%d", &m->numt);
    	getchar();
    	printf("Digite o nome do arquivo de entrada: (.txt)\n");
    	fgets(arq_e, 100,stdin);
    	arq_e[strcspn(arq_e, "\n")]=0;
    	printf("%s\n",arq_e);
   
    	int i, j;
    	inv = (double**) malloc(m->l*sizeof(double*));
    	for(i = 0; i < m->c; i++)
		inv[i] = (double*) malloc(m->c*sizeof(double));

    	pthread_t vetT[m->numt];

    	matriz(m->m, m->l, m->c,arq_e);
    
    	for(i = 0; i < m->numt; i++){
		m->i=i;
		pthread_create(&vetT[i], NULL, inverter, (void *)m);
		printf("%d\n",m->i);
		sleep(1);
	}
    	for(i = 0; i < m->numt; i++){
		pthread_join(vetT[i], NULL);
	}

    	for(i = 0;i < m->c;i++){
        	for(j = 0;j < m->l; j++)
            		printf("%f\t",inv[i][j]);
        	printf("\n");
    	}
    //apagar(inv,m->m,m->l,m->c);
}
