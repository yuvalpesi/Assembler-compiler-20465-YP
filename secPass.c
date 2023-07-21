#include "data.h"

int secPass(char *argv,LineHolder *head, symbolTable *symbol,int IC,int DC){
    LineHolder *current=head;
    EnExNode *Hextern=NULL,*Henty=NULL;
    char *lableName=NULL;
    int num;
    Operand *codeNum=NULL;


    while (head!=NULL){

            if(checkString(head->Binary->lableName)!=False || checkData(head->Binary->lableName)!=False){
                head->address+=IC;
            }
            lableName=symbolTableLockUp(symbol,head->Binary->lableName);

            if(lableName==NULL){
                head=head->next;
                continue;
            }

            num=symbolTableLockUpAddress(symbol,lableName);

            if(symbolTableLockUpType(symbol, lableName) == sEXETRN){
                codeNum=instalBinary(lableName,0,0,0,E);
                replaceNodeItem(head,lableName,codeNum);
            }

            if(num!=ERROR && commandType(lableName)==ERROR ){
                num+=(100);
                if(symbolTableLockUpICDC(symbol, lableName) == sDC){
                    num+=IC;
                }

                codeNum=instalBinary(lableName,num,0,0,R);
                replaceNodeItem(head,lableName,codeNum);
            }

            head=head->next;
    }

    head=current;

    if(((IC+DC)+ADDRESS_START)>RAM_SIZE){
        fprintf(stderr,"Error in file %s: your program size is too big\n",argv);
        return False;
    }

    if(checkEntry(argv,symbol,&Henty,IC)==ERROR){
        freeListNodeEnEx(Henty);
        return False;
    }
    freeListNodeEnEx(Henty);
    Henty=NULL;
    if(checkEntry(argv,symbol,&Henty,IC)==True){
        printEntFile(argv,Henty);
    }

    if(checkExtern(symbol,&Hextern,head)==True){
        printExtFile(argv,Hextern);
    }

    freeListNodeEnEx(Henty);
    freeListNodeEnEx(Hextern);
    return True;
}

int checkEntry(char *argv,symbolTable *table,EnExNode **head,int IC){
    int i,num,type;
    EnExNode *temp=NULL;
    char *lableName=NULL;

    for(i=0;i<TABLE_SIZE;i++){
        symbolTableStart *start=table->entry[i];

        if(start==NULL){
            continue;
        }

        for (;;){
            type=start->symbolType;
            lableName= strDup(start->symbolName);
            num= symbolTableLockUpAddress(table,start->symbolName);

            if(symbolTableLockUp(table,lableName)!=NULL && num==ERROR && type==sENTRY){
                fprintf(stderr,"Error in file %s: The .entry code label %s is not in use\n",argv,lableName);
                freeListNodeEnEx(temp);
                return ERROR;
            }

            if(type==sENTRY && num!=ERROR){
                num+=(100);
                if(symbolTableLockUpICDC(table, lableName) == sDC){
                    num+=IC;
                }

                addNodeEnEx(&temp, createNodeEnEx(lableName, num));

            }

            if(start->next==NULL){
                free(lableName);
                break;
            }
            free(lableName);
            lableName=NULL;
            start=start->next;
        }

    }

    *head=temp;
    if(*head==NULL){
        return False;
    }
    return True;
}

int checkExtern(symbolTable *table,EnExNode **head,LineHolder *curr){
    int num;
    char *lableName=NULL;
    LineHolder *prev=curr;
    EnExNode *temp=NULL;

    while (prev!=NULL){

        lableName=strdup(prev->Binary->lableName);


        if (symbolTableLockUpType(table, prev->Binary->lableName) == sEXETRN) {
            num=prev->address+100;

            addNodeEnEx(&temp, createNodeEnEx(lableName, num));
        }

        free(lableName);
        lableName=NULL;
        prev=prev->next;
    }

    *head=temp;

    if(*head==NULL){
        return False;
    }
    return True;
}

