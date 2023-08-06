#include "data.h"

int secPass(char *argv,LineHolder *head, symbolTable *symbol,int IC,int DC,ExNode **Extern,EnNode **Entry){
    LineHolder *current=head;
    char *lableName=NULL;
    int num,errorCounter=0;
    Operand *codeNum=NULL;

    while (head!=NULL){
        /* if it is a data or sting type add the IC counter to the DC address*/
        if(checkString(head->Binary->lableName)!=False || checkData(head->Binary->lableName)!=False){
            head->address+=IC;
        }

        /* find if the label from the addressing image node is in the table and dup the name*/
        if(symbolTableLockUp(symbol,head->Binary->lableName)!=NULL){
            lableName= strDup(symbolTableLockUp(symbol,head->Binary->lableName));
        }

        if(lableName==NULL){
            head=head->next;
            continue;
        }
        /* num is equal to address variable */
        num=symbolTableLockUpAddress(symbol,lableName); /* get the address from the table that is not entry or extern type */

        if(symbolTableLockUpType(symbol, lableName) == sEXETRN){
            /* carte a new bit-field operand */
            codeNum=installBinary(lableName,0,0,0,E);
            replaceNodeItem(head,lableName,codeNum);/* replace with new address location*/
        }

        if(num!=ERROR && commandType(lableName)==ERROR ){
            num+=(100);
            /* if the label is data image add to the address the IC counter*/
            if(symbolTableLockUpICDC(symbol, lableName) == sDC){
                num+=IC;
            }
            /* carte a new bit-field operand */
            codeNum=installBinary(lableName,num,0,0,R);
            replaceNodeItem(head,lableName,codeNum);/* replace with new address location*/
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

    /* crate an entry node if there is one in the file and if not return null list */
    checkEntry(Entry,argv,symbol,IC,&errorCounter);

    /* crate an extern node if there is one in the file and if not return null list */
    checkExtern(Extern,symbol,&head,errorCounter);

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

            /* if the is label that is not in used send error */
            if(symbolTableLockUp(table,labelName)!=NULL && num==ERROR && type==sENTRY){
                fprintf(stderr,"Error in file %s: The .entry code label %s is not in use\n",argv,labelName);
                (*errorsCounter)++;
                if(head!=NULL){
                    freeListNodeEn(head);
                }
                return;
            }
            /* see if the symbol type is entry and address is not -1(error)
               it mean that entry label is in used */
            if(type==sENTRY && num!=ERROR){
                num+=(100);
                /* if the label is data image add to the address the IC counter*/
                if(symbolTableLockUpICDC(table, labelName) == sDC){
                    num+=IC;
                }
                /* crate and add the entry location and name in to the entry linked list node*/
                addNodeEn(&head, createNodeEn(labelName, num));
            }

            if(start->next==NULL){
                break;
            }
            start=start->next;
        }
    }

    /* if the head(entry node) is not empty
        set the linked list to the Entry pointer*/
    if(head!=NULL) {
        *Entry=head;
    }
}

void checkExtern(ExNode **Extern,symbolTable *table,LineHolder **curr,int errorsCounter){
    int num;
    LineHolder *prev=*curr;
    ExNode *head=NULL;
    char tempStr[MAX_LABLE]= {0};

    /* check the addressing image and see if extern exists in it */
    while (prev!=NULL){
        strcpy(tempStr,prev->Binary->lableName);
        /* see if exists the label in the symbol table and if is type is extern */
        if (symbolTableLockUpType(table, prev->Binary->lableName) == sEXETRN) {
            num=prev->address+100;
            /* crate and add the extern location and name in to the extern linked list node*/
            addNodeEx(&head, createNodeEx(tempStr, num));
        }
        prev=prev->next;
    }

    /* if the head(extern node) is not empty and there is no errors
        set the linked list to the Extern pointer*/
    if(head!=NULL && errorsCounter==0){
        *Extern=head;
    }
}
