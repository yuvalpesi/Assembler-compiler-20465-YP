#include "data.h"

LineHolder* createNodeItem(int address,Operand *code){
    LineHolder *node = (LineHolder*)malloc(sizeof(LineHolder)); /* allocates memory for the node */

    if (node==NULL){
        printf(" Error: unable to allocate memory for new node\n");
        return NULL;
    }

    /* Set the data and next pointer of the node */
    node->address=address;
    node->Binary=code;
    node->next = NULL;
    return node;
}

void addNode(LineHolder **head, LineHolder *nodeItem){
    /*  If the linked list is empty, set the head to point to the new node */
    if (*head==NULL){
        *head = nodeItem;
        nodeItem->next = NULL;
    }else{/* If the linked list already exists */
        LineHolder *current = *head; /*Create a current pointer to traverse the list */

        /* Find the last node in the circular list */
        while(current->next!=NULL) {
            current = current->next;
        }
        current->next = nodeItem; /* Set the new node as the next node of the last node */
        nodeItem->next = NULL; /* Maintain the circular list by setting the next pointer of the new node to the head */
    }
}

void freeListNode(LineHolder *head){
    LineHolder *temp=NULL;

    while(head!=NULL){
        temp=head->next;
        free(head->Binary->lableName);
        free(head->Binary);
        free(head);
        head=temp;

        if(head==temp){
            break;
        }
    }
}
