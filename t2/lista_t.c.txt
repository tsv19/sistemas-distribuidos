#include "lista_t.h"

Node *createNode(int processo)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
    {
        printf("Erro de alocacao de memoria\n");
        exit(1);
    }

    newNode->processo = processo;
    newNode->sFilho = 0;
    newNode->next = NULL;
    return newNode;
}

Node *createNodeProcesso(int processo, int sFilho)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
    {
        printf("Erro de alocacao de memoria\n");
        exit(1);
    }

    newNode->processo = processo;
    newNode->sFilho = sFilho;
    newNode->next = NULL;
    return newNode;
}

void insertAtHeadProcesso(Node **head, int processo, int sFilho)
{
    Node *newNode = createNode(processo);
    newNode->next = *head;
    *head = newNode;
}

void insertAtTailProcesso(Node **head, int processo, int sFilho)
{
    Node *newNode = createNodeProcesso(processo, sFilho);
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }
    Node *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
}

void deleteNode(Node **head, int key)
{
    Node *temp = *head;
    Node *prev = NULL;

    if (temp != NULL && temp->processo == key)
    {
        *head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->processo != key)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
        return;

    prev->next = temp->next;
    free(temp);
}

void printList(Node *head)
{
    Node *temp = head;
    while (temp != NULL)
    {
        printf("%d -> ", temp->processo);
        temp = temp->next;
    }
    printf("NULL\n");
}

Node *search(Node *head, int key)
{
    Node *current = head;
    while (current != NULL)
    {
        if (current->processo == key)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void deleteList(Node **head)
{
    Node *current = *head;
    Node *nextNode = NULL;

    while (current != NULL)
    {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    *head = NULL;
}

Node *removeFirstNode(Node **head)
{
    if (*head == NULL)
    {
        return NULL;
    }

    Node *temp = *head;
    *head = (*head)->next;

    return temp;
}