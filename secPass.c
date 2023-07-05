#include "data.h"

int secPass(char *argv,LineHolder *head, symbolTable *symbol,int IC,int DC){
    LineHolder *current=head;
    LineHolder *Hextern=NULL,*Henty=NULL;
    char *lableName=NULL;
    int num;
    Operand *codeNum=NULL;


    while (head!=NULL){
            lableName=symbolTableLockUp(symbol,head->Binary->lableName);

            if(lableName==NULL){
                head=head->next;
                continue;
            }

           num=symbolTableLockUpAddress(symbol,lableName);

            if(symbolTableLockUpType(symbol, lableName) == sEXETRN){
                codeNum=instalBinary(codeNum,lableName,0,0,0,E);
                replaceNodeItem(head,lableName,codeNum);
            }

            if(num!=-2 && commandType(lableName)==ERROR ){
                codeNum=instalBinary(codeNum,lableName,num,0,0,R);
                replaceNodeItem(head,lableName,codeNum);

            }

            head=head->next;
    }

    head=current;

    if(((IC+DC)+ADDRESS_START)>RAM_SIZE){
        printf("Error: your program size is too big\n");
        return False;
    }

    if(checkEntry(symbol,&Henty)==True){
        printEntFile(argv,Henty);
    }

    if(checkExtern(symbol,&Hextern,head)==True){
        printExtFile(argv,Hextern);
    }

    freeListNode(Henty);
    freeListNode(Hextern);
    return True;
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

int checkEntry(symbolTable *table,LineHolder **head){
    int i,num,type;
    Operand *codeNum=NULL;
    LineHolder *temp=NULL;

    for(i=0;i<TABLE_SIZE;i++){
        symbolTableStart *start=table->entry[i];

        if(start==NULL){
            continue;
        }

        for (;;){
            type=start->symbolType;
            num= symbolTableLockUpAddress(table,start->symbolName);

            if(type==sENTRY && num!=-2){

                codeNum = instalBinary(codeNum, start->symbolName, 0, 0, 0,0);
                addNode(&temp, createNodeItem(num, codeNum));

            } else if(type==sEXETRN && num!=-2){
                codeNum = instalBinary(codeNum, start->symbolName, 0, 0, 0,0);
                addNode(&temp, createNodeItem(num, codeNum));
            }

            if(start->next==NULL){
                break;
            }
            start=start->next;
        }

    }

    *head=temp;

    if(*head==NULL){
        return False;
    }
    return True;
}

int checkExtern(symbolTable *table,LineHolder **head,LineHolder *curr){
    int num;
    char *lableName=NULL;
    Operand *codeNum=NULL;
    LineHolder *temp=NULL;

    while (curr!=NULL){

        lableName=curr->Binary->lableName;


        if (symbolTableLockUpType(table, lableName) == sEXETRN) {
            num=curr->address;

            codeNum = instalBinary(codeNum, lableName, 0, 0, 0,0);

            addNode(&temp, createNodeItem(num, codeNum));

        }

        curr=curr->next;
    }

    *head=temp;

    if(*head==NULL){
        return False;
    }
    return True;
}

