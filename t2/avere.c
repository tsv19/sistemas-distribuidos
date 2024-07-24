#include <stdio.h>
#include "avere.h"
#include "cisj.h"
#include "lista_t.h"

Node *filaProcessos = NULL;

void subArvere(int processoId, int sFilho, Node *listaProcessosFalhos);

void arvere(int raizId, int dimensaoVCube, Node *listaProcessosFalhos)
{
    printf("Sou o processo %d e sou Raiz da Arvore Geradora Minima!\n", raizId);

    // Vamos enviar uma mensagem para o primeiro processo correto de cada nível
    for (int i = 1; i <= dimensaoVCube; i++)
    {
        node_set *nodesCandidatos = cis(raizId, i);
        int primeiroCorreto = -1;

        // Procura o primeiro processo correto do nível i
        for (int j = 0; j < nodesCandidatos->size; j++)
        {
            int candidato = nodesCandidatos->nodes[j];

            // printf("Candidato: %d\n", candidato);

            if (listaProcessosFalhos != NULL)
            {
                Node *processo = search(listaProcessosFalhos, candidato);

                if (processo != NULL)
                {
                    continue;
                }
            }

            primeiroCorreto = candidato;
            break;
        }

        // Não existe processo correto no nível s, ignoramos o nível e vamos para o próximo
        if (primeiroCorreto == -1)
        {
            printf("Processo %d não encontrou processo correto no nível %d\n", raizId, i);
            continue;
        }

        printf("Processo %d enviando mensagem para o processo %d com s-filho %d\n", raizId, primeiroCorreto, i);
        // envia(primeiroCorreto, i, listaProcessosFalhos);

        insertAtTailProcesso(&filaProcessos, primeiroCorreto, i);

        set_free(nodesCandidatos);
    }

    printf("\n");

    for (int i = 0; i < dimensaoVCube; i++)
    {
        Node *targetNode = removeFirstNode(&filaProcessos);
        if (targetNode == NULL)
        {
            break;
        }
        int target = targetNode->processo;
        int sFilho = targetNode->sFilho;
        subArvere(target, sFilho, listaProcessosFalhos);
    }

    // printf("Sou o processo %d e terminei de enviar mensagens para meus filhos!\n", raizId);

    deleteList(&filaProcessos);
}

void subArvere(int processoId, int sFilho, Node *listaProcessosFalhos)
{

    // printf("Sou o processo %d e tenho s-filho %d\n", processoId, sFilho);

    if (sFilho == 1)
    {
        printf("Sou o processo %d e sou Folha!\n\n", processoId);
        return;
    }

    // Vamos enviar uma mensagem para o primeiro processo correto de cada nível
    for (int i = 1; i <= sFilho - 1; i++)
    {
        node_set *nodesCandidatos = cis(processoId, i);
        int primeiroCorreto = -1;

        // Procura o primeiro processo correto do nível i
        for (int j = 0; j < nodesCandidatos->size; j++)
        {
            int candidato = nodesCandidatos->nodes[j];

            if (listaProcessosFalhos != NULL)
            {
                Node *processo = search(listaProcessosFalhos, candidato);

                if (processo != NULL)
                {
                    continue;
                }
            }

            primeiroCorreto = candidato;
            break;
        }

        // Não existe processo correto no nível s, ignoramos o nível e vamos para o próximo
        if (primeiroCorreto == -1)
        {
            printf("Processo %d não encontrou processo correto no nível %d\n", processoId, i);
            continue;
        }

        printf("Processo %d enviando mensagem para o processo %d com s-filho %d\n", processoId, primeiroCorreto, i);
        insertAtTailProcesso(&filaProcessos, primeiroCorreto, i);

        set_free(nodesCandidatos);
    }

    printf("\n");

    for (int i = 1; i < sFilho; i++)
    {
        Node *targetNode = removeFirstNode(&filaProcessos);
        if (targetNode == NULL)
        {
            break;
        }
        int target = targetNode->processo;
        int sFilho = targetNode->sFilho;

        subArvere(target, sFilho, listaProcessosFalhos);
    }

    // printf("Sou o processo %d e terminei de enviar mensagens para meus filhos!\n", processoId);
}

// int main()
// {

//     int raizId = 0;
//     int dimensaoVCube = 3;

//     Node *listaProcessosFalhos = NULL;
//     // Node *listaProcessosFalhos = createNode(1);
//     // Node *listaProcessosFalhos = createNode(2);
//     // Node *listaProcessosFalhos = createNode(4);
//     // Node *listaProcessosFalhos = createNode(7);

//     arvere(raizId, dimensaoVCube, listaProcessosFalhos);
//     deleteList(&listaProcessosFalhos);
// }