#include "data.h"

int secPass(char *argv,LineHolder *head, symbolTable *symbol,int IC,int DC){
    LineHolder *current=head;
    char *lableName=NULL;
    int num;
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
        return False;
    }

    checkEntry(argv,symbol,IC);
    checkExtern(argv,symbol,&head);

    return True;
}

void checkEntry(char *argv,symbolTable *table,int IC){
    int i,num,type;
    EnExNode *temp=NULL;
    char *labelName=NULL;

    for(i=0;i<TABLE_SIZE;i++){
        symbolTableStart *start=table->entry[i];

        if(start==NULL){
            continue;
        }

        for (;;){
            type=start->symbolType;
            num= symbolTableLockUpAddress(table,start->symbolName);

            if(symbolTableLockUp(table,start->symbolName)!=NULL && num==ERROR && type==sENTRY){
                fprintf(stderr,"Error in file %s: The .entry code label %s is not in use\n",argv,start->symbolName);
                freeListNodeEnEx(temp);
                return;
            }

            if(type==sENTRY && num!=ERROR){
                if(labelName!=NULL){
                free(labelName);
                labelName=NULL;
            }
                labelName= strDup(start->symbolName);
                num+=(100);
                if(symbolTableLockUpICDC(table, labelName) == sDC){
                    num+=IC;
                }

                addNodeEnEx(&temp, createNodeEnEx(labelName, num));
            }

            if(start->next==NULL){

                break;
            }

            start=start->next;
        }
    }

    if(temp==NULL){
        return;
    }else{
        printEntFile(argv,temp);
        freeListNodeEnEx(temp);
        if(labelName!=NULL)free(labelName);
    }
}

void checkExtern(char *argv,symbolTable *table,LineHolder **curr){
    int num;
    LineHolder *prev=*curr;
    EnExNode *temp=NULL;
    char *tempStr=NULL;

    while (prev!=NULL){

        if (symbolTableLockUpType(table, prev->Binary->lableName) == sEXETRN) {
            if(tempStr!=NULL){
                free(tempStr);
                tempStr=NULL;
            }
            tempStr= strDup(prev->Binary->lableName);
            num=prev->address+100;

            addNodeEnEx(&temp, createNodeEnEx(tempStr, num));
        }

        prev=prev->next;
    }

    if(temp==NULL){
        return;
    } else{
        printExtFile(argv,temp);
        freeListNodeEnEx(temp);
        if(tempStr!=NULL)free(tempStr);
    }
}

