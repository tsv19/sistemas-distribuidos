/* Autor: Elias P. Duarte Jr.
   Data da Ãšltima ModificaÃ§Ã£o: 11/abril/2024
   DescriÃ§Ã£o: Nosso primeiro programa de simulaÃ§Ã£o da disciplina Sistemas DistribuÃ­dos.
     Vamos simular N processos, cada um conta o â€œtempoâ€ independentemente
    Um exemplo simples e significativo para captar o â€œespÃ­ritoâ€ da simulaÃ§Ã£o */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "smpl.h"
#include "cisj.c"

// Vamos definir os EVENTOS
#define test 1
#define fault 2
#define recovery 3

// Vamos definir o descritor do processo

typedef struct
{
    int id; // identificador de facility do SMPL
            // outras variÃ¡veis locais dos processos sÃ£o declaradas aqui!

    int *state; // 0 = correto, 1 = falho, -1 = unknown
    int rodada;
} TipoProcesso;

TipoProcesso *processo;

int main(int argc, char *argv[])
{

    static int N, // nÃºmero de processos
        token,    // indica o processo que estÃ¡ executando
        event, r, i,
        MaxTempoSimulac = 180;

    static char fa_name[5];

    if (argc != 2)
    {
        puts("Uso correto: tempo <nÃºmero de processos>");
        exit(1);
    }

    N = atoi(argv[1]);

    smpl(0, "Um Exemplo de SimulaÃ§Ã£o");
    reset();
    stream(1);

    // inicializar processos

    processo = (TipoProcesso *)malloc(sizeof(TipoProcesso) * N);

    for (i = 0; i < N; i++)
    {
        memset(fa_name, '\0', 5);
        sprintf(fa_name, "%d", i);
        processo[i].id = facility(fa_name, 1);

        processo[i].state = (int *)malloc(sizeof(int) * N);
        for (int j = 0; j < N; j++)
        {
            processo[i].state[j] = -1;
        }

        processo[i].state[i] = 0;
        processo[i].rodada = 1;
    }
    // vamos fazer o escalonamento inicial de eventos

    // nossos processos vÃ£o executar testes em intervalos de testes
    // o intervalo de testes vai ser de 30 unidades de tempo
    // a simulaÃ§Ã£o comeÃ§a no tempo 0 (zero) e vamos escalonar o primeiro teste de todos os
    //       processos para o tempo 30.0

    for (i = 0; i < N; i++)
    {
        schedule(test, 30.0, i);
    }
    schedule(fault, 31.0, 0);
    schedule(recovery, 91.0, 0);

    // agora vem o loop principal do simulador

    puts("===============================================================");
    puts("           Sistemas Distribuídos Prof. Elias");
    puts("               LOG do Trabalho Prático 1");
    puts("     Implementar o detector de falhas vCube (versão 2)");
    printf("      Este programa foi executado para: N=%d processos.\n", N);
    printf("           Tempo Total de Simulação = %d\n", MaxTempoSimulac);
    puts("===============================================================");

    double tempoAnterior = time();
    while (time() < MaxTempoSimulac)
    {
        cause(&event, &token);

        if (time() != tempoAnterior)
        {
            printf("\n");
            tempoAnterior = time();
        }

        switch (event)
        {
        case test:
            int testador = token;
            node_set *nodes;
            node_set *nodesTestador;

            processo[token].rodada++;

            if (status(processo[testador].id) != 0)
                break; // se o processo estÃ¡ falho, nÃ£o testa!

            // c(i, s) retorna o(s) testador(es) do processo i no nível s
            // executar c(i, s) para todos os processos (menos o testador). EM ORDEM DO HIPERCUBO (ou seja, C(testador, s) para cada nivel s)
            // se o testador for o primeiro processo valido em algum nivel s, entao testa o processo i
            // se i for valido, copiar vetor state com novidades (Se o valor da entrada for maior no vetor STATE do processo testado, então copia a informação)

            printf("Tempo %5.1f: Processo %d testou:  ", time(), testador);

            int s = ceil(log2(N));
            for (int nivelTestador = 1; nivelTestador <= s; nivelTestador++)
            {
                nodesTestador = cis(testador, nivelTestador);

                for (int jTestador = 0; jTestador < nodesTestador->size; jTestador++)
                {
                    int testado = nodesTestador->nodes[jTestador];
                    if (testado == testador || testado > N)
                        continue;

                    for (int nivel = 1; nivel <= s; nivel++)
                    {
                        // printf("Testador: %d, Testado: %d, Nivel: %d\n", testador, testado, nivel);
                        nodes = cis(testado, nivel);
                        int primeiroCorreto;

                        for (int j = 0; j < nodes->size; j++)
                        {
                            primeiroCorreto = nodes->nodes[j];

                            if (primeiroCorreto > N)
                                continue;

                            if (abs(processo[testador].state[primeiroCorreto] % 2) == 0 || processo[testador].state[primeiroCorreto] == -1)
                                break;
                        }

                        // LEMBRAR
                        // ESTADO PAR = CORRETO
                        // ESTADO IMPAR = FALHO
                        if (primeiroCorreto == testador)
                        {
                            if (status(processo[testado].id) != 0)
                            {
                                printf("%d - FALHO!  , ", testado);
                                if (processo[testador].state[testado] % 2 == 0)
                                {
                                    processo[testador].state[testado] += 1;
                                }
                            }
                            else
                            {
                                printf("%d - Sucesso!, ", testado);

                                if (abs(processo[testador].state[testado] % 2) == 1)
                                {
                                    processo[testador].state[testado] += 1;
                                }
                                for (int i = 0; i < N; i++)
                                {
                                    if (processo[testado].state[i] > processo[testador].state[i] && i != testador)
                                    {
                                        processo[testador].state[i] = processo[testado].state[i];
                                    }
                                }
                            }
                        }

                        set_free(nodes);
                    }
                }

                set_free(nodesTestador);
            }

            schedule(test, 30.0, testador);

            printf("State: [");
            for (int i = 0; i < N; i++)
            {
                printf(" %2d ", processo[testador].state[i]);
            }
            printf("]\n");

            break;
        case fault:
            r = request(processo[token].id, token, 0);
            printf("Socooorro!!! Sou o processo %d  e estou falhando no tempo %4.1f\n", token, time());
            break;
        case recovery:
            release(processo[token].id, token);
            printf("Viva!!! Sou o processo %d e acabo de recuperar no tempo %4.1f\n", token, time());
            schedule(test, 1.0, token);
            break;
        } // switch
    }
    // while

} // tempo.c
