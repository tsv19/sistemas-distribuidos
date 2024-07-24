#ifndef __LISTA_T__
#define __LISTA_T__
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int processo;
    int sFilho;
    struct Node *next;
} Node;

Node *createNode(int processo);
Node *createNodeProcesso(int processo, int sFilho);
void insertAtHeadProcesso(Node **head, int processo, int sFilho);
void insertAtTailProcesso(Node **head, int processo, int sFilho);
void deleteNode(Node **head, int key);
void printList(Node *head);
Node *search(Node *head, int key);
void deleteList(Node **head);
Node *removeFirstNode(Node **head);

#endif