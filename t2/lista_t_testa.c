#include "lista_t.h"

int main()
{
    Node *head = NULL;

    insertAtHead(&head, 1);
    insertAtHead(&head, 2);
    insertAtHead(&head, 3);

    printf("Lista apos insercoes no inicio: ");
    printList(head);

    insertAtTail(&head, 4);
    insertAtTail(&head, 5);

    printf("Lista apos insercoes no fim: ");
    printList(head);

    deleteNode(&head, 3);

    printf("Lista apos insercoes do valor 3: ");
    printList(head);

    int key = 3;
    Node *result = search(head, key);

    if (result != NULL)
    {
        printf("Valor %d encontrado na lista.\n", key);
    }
    else
    {
        printf("Valor %d não encontrado na lista.\n", key);
    }

    int key2 = 4;
    Node *result2 = search(head, key2);

    if (result2 != NULL)
    {
        printf("Valor %d encontrado na lista.\n", key2);
    }
    else
    {
        printf("Valor %d não encontrado na lista.\n", key2);
    }

    deleteNode(&head, 3);

    printf("Lista apos exclusao do valor 3: ");
    printList(head);

    return 0;
}