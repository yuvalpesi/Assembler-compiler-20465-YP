#include "data.h"

LineHolder* createNodeItem(int address,Operand *code){
    LineHolder *node = (LineHolder*)malloc(sizeof(LineHolder)); /* allocates memory for the node */

    if (node==NULL){
        printf("Error: unable to allocate memory for new node\n");
        exit(EXIT_FAILURE);
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

        /* Find the last node in the linked list that is data image*/
        if(checkString(nodeItem->Binary->lableName)==True || checkData(nodeItem->Binary->lableName)==True){
            while(current->next!=NULL ) {
                current = current->next;
            }
            current->next = nodeItem; /* Set the new node as the next node of the last node */
            nodeItem->next = NULL; /* Maintain the linked list by setting the next pointer of the new node to the null */
        } else {

            if(current->next==NULL){
                current->next = nodeItem; /* Set the new node as the next node of the last node */
                nodeItem->next = NULL; /* Maintain the linked list by setting the next pointer of the new node to the null */
            }else {

                LineHolder *prev = *head;
                current=current->next;
                /* Find the last node in the linked list that is a code image*/
                while (current!=NULL && current->next!=NULL) {
                    prev = current;
                    current = current->next;
                    if(checkData(current->Binary->lableName)!=False || checkString(current->Binary->lableName)!=False){
                        break;
                    }
                }

                if (current->next == NULL && checkData(current->Binary->lableName)==False && checkString(current->Binary->lableName)==False) {
                    current->next = nodeItem; /* Set the new node as the next node of the last node */
                    nodeItem->next = NULL; /* Maintain the linked list by setting the next pointer of the new node to the null */
                } else {
                    prev->next= nodeItem; /* add the node item to the right place*/
                    nodeItem->next = current; /* connect the rest of the node */
                }
            }

        }

    }
}

void replaceNodeItem(LineHolder * head, char *item, Operand *code) {
    LineHolder * current = head;

    while (current != NULL) {
        if (strcmp(current->Binary->lableName,item)==0) {
            freeBinery(current->Binary);
            free(current->Binary->lableName);
            free(current->Binary);
            current->Binary = code;
            break;
        }
        current = current->next;
    }
}

void freeListNode(LineHolder *head){
    LineHolder *temp=NULL;

    while(head!=NULL){
        temp=head->next;
        freeBinery(head->Binary); /* free all the binary struct */
        free(head->Binary->lableName);
        free(head->Binary);
        free(head);
        head=temp;
    }
    free(head);
    free(temp);
}

lineStr* createNodeLine(char *line){
    lineStr *node = (lineStr*)malloc(sizeof(lineStr)); /* allocates memory for the node */

    if (node==NULL){
        printf("Error: unable to allocate memory for new node\n");
        exit(EXIT_FAILURE);
    }

    /* Set the data and next pointer of the node */
    node->lineStr= strDup(line);
    node->next = NULL;
    return node;
}

void addNodeLine(lineStr **head, lineStr *nodeItem){
    if (*head==NULL){
        *head = nodeItem;
        nodeItem->next = NULL;
    }else{/* If the linked list already exists */
        lineStr *current = *head; /*Create a current pointer to traverse the list */

        /* Find the last node in the linked list */
        while(current->next!=NULL) {
            current = current->next;
        }
        current->next = nodeItem; /* Set the new node as the next node of the last node */
        nodeItem->next = NULL; /* Maintain the linked list by setting the next pointer of the new node to the null */
    }
}

void freeListNodeLine(lineStr *head){
    lineStr *temp=NULL;

    while(head!=NULL){
        temp=head->next;
        free(head->lineStr);
        free(head);
        head=temp;
    }
}

EnNode* createNodeEn(const char *line,int address){
    EnNode *node = (EnNode*)malloc(sizeof(EnNode)); /* allocates memory for the node */

    if (node==NULL){
        printf("Error: unable to allocate memory for new node\n");
        exit(EXIT_FAILURE);
    }

    /* Set the data and next pointer of the node */
    strcpy(node->lable,line);
    node->address=address;
    node->next = NULL;
    return node;
}

void addNodeEn(EnNode **head, EnNode *nodeItem){
    if (*head==NULL){
        *head = nodeItem;
        nodeItem->next = NULL;
    }else{/* If the linked list already exists */
        EnNode *current = *head; /*Create a current pointer to traverse the list */

        /* Find the last node in the linked list */
        while(current->next!=NULL) {
            current = current->next;
        }
        current->next = nodeItem; /* Set the new node as the next node of the last node */
        nodeItem->next = NULL; /* Maintain the linked list by setting the next pointer of the new node to the null */
    }
}

void freeListNodeEn(EnNode *head){
    EnNode *temp=NULL;

    if(head==NULL){
        return;
    }

    while(head!=NULL){
        temp=head->next;
        free(head);
        head=temp;
    }
}

ExNode* createNodeEx(const char *line,int address){
    ExNode *node = (ExNode*)malloc(sizeof(ExNode)); /* allocates memory for the node */

    if (node==NULL){
        printf("Error: unable to allocate memory for new node\n");
        exit(EXIT_FAILURE);
    }

    /* Set the data and next pointer of the node */
    strcpy(node->lable,line);
    node->address=address;
    node->next = NULL;
    return node;
}

void addNodeEx(ExNode **head, ExNode *nodeItem){
    if (*head==NULL){
        *head = nodeItem;
        nodeItem->next = NULL;
    }else{/* If the linked list already exists */
        ExNode *current = *head; /*Create a current pointer to traverse the list */

        /* Find the last node in the linked list */
        while(current->next!=NULL) {
            current = current->next;
        }
        current->next = nodeItem; /* Set the new node as the next node of the last node */
        nodeItem->next = NULL; /* Maintain the linked list by setting the next pointer of the new node to the null */
    }
}

void freeListNodeEx(ExNode *head){
    ExNode *temp=NULL;

    if(head==NULL){
        return;
    }

    while(head!=NULL){
        temp=head->next;
        free(head);
        head=temp;
    }
}
