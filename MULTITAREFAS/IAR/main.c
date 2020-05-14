#include <stdint.h>
#include <stdio.h>

#include "multitarefas.h"

/*
 * Prototipos das tarefas
 */
void produtor(void);
void consumidor(void);
int produz();
int consome(int *buffer);

/*
 * Configuracao dos tamanhos das pilhas
 */
#define TAM_PILHA_1		(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_2		(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_3		(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_OCIOSA	(TAM_MINIMO_PILHA + 24)

#define N_ELEMENTOS 5

/*
 * Declaracao das pilhas das tarefas
 */
uint32_t PILHA_TAREFA_1[TAM_PILHA_1];
uint32_t PILHA_TAREFA_2[TAM_PILHA_2];
uint32_t PILHA_TAREFA_3[TAM_PILHA_3];
uint32_t PILHA_TAREFA_OCIOSA[TAM_PILHA_OCIOSA];



/* Declara Semaforos*/
semaforo_t cheio = 0;
semaforo_t vazio = N_ELEMENTOS;


int buffer[N_ELEMENTOS];

/*
 * Funcao principal de entrada do sistema
 */
int main(void)
{
	
	/* Criacao das tarefas */
	/* Parametros: ponteiro, nome, ponteiro da pilha, tamanho da pilha, prioridade da tarefa */
	
	CriaTarefa(produtor, "Tarefa 1", PILHA_TAREFA_1, TAM_PILHA_1, 2);
	
	CriaTarefa(consumidor, "Tarefa 2", PILHA_TAREFA_2, TAM_PILHA_2, 1);
	
	/* Cria tarefa ociosa do sistema */
	CriaTarefa(tarefa_ociosa,"Tarefa ociosa", PILHA_TAREFA_OCIOSA, TAM_PILHA_OCIOSA, 0);
	
	/* Configura marca de tempo */
	ConfiguraMarcaTempo();   
	
	/* Inicia sistema multitarefas */
	IniciaMultitarefas();
	
	/* Nunca chega aqui */
	while (1)
	{
	}
}

/* Tarefas de exemplo que usam funcoes para suspender/continuar as tarefas */

void produtor(){
  int f = 4;
  while(1){
    SemaforoAguarda(&vazio);
    /* O q acontece qndo se faz wait(vazio) ??
       se cont < 0, cont do sem_vazio ser� decrementado. Indicando que h� menos espa�o no buffer 
       Sen�o, a tarefa produtora ser� referenciada no sem_cheio e haver� uma troca de contexto. Indicando que o buffer est� cheio
       Se o else n�o for satisfeito a tarefa produtora contitua suas instru��es
    */
    f = (f + 1)%N_ELEMENTOS;
    buffer[f] = produz();
    SemaforoLibera(&cheio);
    //TarefaSuspende(1);
    /* O q acontece qndo se faz signal(cheio) ??
       1� verifica-se se existe uma tarefa esperando em sem_cheio
             Se sim, muda-se o estada da tarefa para pronta. Indicando que a tarefa consumidora foi susspensa pq tentou usar o buffer sem ele estar livre
             Sen�o, contador de sem_cheio � incrementado indicando q o buffer n�o est� sendo usado pelo produtor
       2� Ocorre uma troca de contexto
    */
  }
}

void consumidor(){
  int i = 4;
  int leitura;
  while(1){
    SemaforoAguarda(&cheio);
    /* O q acontece qndo se faz wait(cheio) ??
       se cont < 0, cont do sem_cheio ser� decrementado. Indicando que o buffer esta sendo usado pelo consumidor
       Sen�o, a tarefa consumidora ser� referenciada no sem_cheio e haver� uma troca de contexto. Indicando que o buffer esta sendo usado pelo produdor
       Se o else n�o for satisfeito a tarefa consumidora contitua suas instru��es
    */
    i = (i + 1)%N_ELEMENTOS;
    leitura = consome(buffer+i);
    SemaforoLibera(&vazio);
    //TarefaSuspende(2);
    /* O q acontece qndo se faz signal(vazio) ??
       1� verifica-se se existe uma tarefa esperando em sem_vazio
             Se sim, muda-se o estada da tarefa para pronta. E produz-se mais um elemento
             Sen�o, contador de sem_vazio � incrementado indicando q h� mais um elemento no buffer
       2� Ocorre uma troca de contexto
    */
  }
}

int produz(){
  static int numero = 0;
  numero++;
  return numero;
}

int consome(int *buffer){
  int aux;
  aux = *buffer;
  *buffer = NULL;
  return aux;
}