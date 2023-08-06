#include "data.h"

int secPass(char *argv,LineHolder *head, symbolTable *symbol,int IC,int DC,ExNode **Extern,EnNode **Entry){
    LineHolder *current=head;
    char *lableName=NULL;
    int num,errorCounter=0;
    Operand *codeNum=NULL;


    while (head!=NULL){

        if(checkString(head->Binary->lableName)!=False || checkData(head->Binary->lableName)!=False){
            head->address+=IC;
        }
        if(symbolTableLockUp(symbol,head->Binary->lableName)!=NULL){
            lableName= strDup(symbolTableLockUp(symbol,head->Binary->lableName));
        }

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

        free(lableName);
        lableName=NULL;
        head=head->next;
    }

    head=current;

    if(((IC+DC)+ADDRESS_START)>RAM_SIZE){
        fprintf(stderr,"Error in file %s: your program size is too big\n",argv);
        errorCounter++;
    }

    checkEntry(Entry,argv,symbol,IC,&errorCounter);
    checkExtern(Extern,argv,symbol,&head,errorCounter);

    return errorCounter==0?True:False;
}

void checkEntry(EnNode **Entry,char *argv,symbolTable *table,int IC,int *errorsCounter){
    int i,num,type;
    EnNode *head=NULL;
    char labelName[MAX_LABLE]= {0};

    for(i=0;i<TABLE_SIZE;i++){
        symbolTableStart *start=table->entry[i];

        if(start==NULL){
            continue;
        }

        for (;;){
            strcpy(labelName,start->symbolName);
            type=start->symbolType;
            num= symbolTableLockUpAddress(table,labelName);

            if(symbolTableLockUp(table,labelName)!=NULL && num==ERROR && type==sENTRY){
                fprintf(stderr,"Error in file %s: The .entry code label %s is not in use\n",argv,labelName);
                (*errorsCounter)++;
                if(head!=NULL){
                    freeListNodeEn(head);
                }
                return;
            }

            if(type==sENTRY && num!=ERROR){
                num+=(100);
                if(symbolTableLockUpICDC(table, labelName) == sDC){
                    num+=IC;
                }

                addNodeEn(&head, createNodeEn(labelName, num));
            }

            if(start->next==NULL){
                break;
            }
            start=start->next;
        }
    }

    if(head!=NULL) {
        *Entry=head;
    }
}

void checkExtern(ExNode **Extern,char *argv,symbolTable *table,LineHolder **curr,int errorsCounter){
    int num;
    LineHolder *prev=*curr;
    ExNode *head=NULL;
    char tempStr[MAX_LABLE]= {0};

    while (prev!=NULL){
        strcpy(tempStr,prev->Binary->lableName);
        if (symbolTableLockUpType(table, prev->Binary->lableName) == sEXETRN) {
            num=prev->address+100;
            addNodeEx(&head, createNodeEx(tempStr, num));
        }

        prev=prev->next;
    }

    if(head!=NULL && errorsCounter==0){
        *Extern=head;
    }
}

