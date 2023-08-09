#include "data.h"

int getFile(int argc,char **argv){
    int i=1;

    if (argc==1){
        fprintf(stderr, "\n Error: You did not send any files to the compiler \n");
        return EXIT_FAILURE;
    }

    while(--argc){
        setFile(argv[i]);
        i++;
    }
    return 0;
}

void setFile(char *argv){
    int IC=0,DC=0;
    symbolTable *symbol=initSymbolTable();
    LineHolder *head=NULL;
    EnNode *Entry=NULL;
    ExNode *Extern=NULL;
    char *str=NULL;

    str= strDup(argv);/* copy the file name */
    str[strlen(str)]='\0';

    /* Spread the macros from the input file */
    if(!preProccesor(str)){
        printf("Couldn't spread the macros correctly into %s\n", str);
        freeSymbolTable(symbol);/* free the symbol table */
        free(str);/* free the file name */
        return;
    }

    /* check the input file (.am) after preprocessor and build the addressing image Node */
    if(firstPass(str,&head,symbol,&IC,&DC)!=True){
        /* find more error in the sec pass */
        secPass(str,head,symbol,IC,DC,&Extern,&Entry);
        printf("Couldn't compile the file %s\n", str);
        freeListNode(head);/* free the addressing image node */
        freeSymbolTable(symbol);/* free the symbol table */
        freeListNodeEn(Entry);/* free the Entry image node */
        freeListNodeEx(Extern);/* free the Extern image node */
        free(str);/* free the file name */
        return;
    }

    /* check addressing image node and set all the unknown data with the new address and build the entry and extern data node if exists */
    if(secPass(str,head,symbol,IC,DC,&Extern,&Entry)!=True){
        printf("Couldn't compile the file %s\n", str);
        freeListNode(head);/* free the addressing image node */
        freeSymbolTable(symbol);/* free the symbol table */
        freeListNodeEn(Entry);/* free the Entry image node */
        freeListNodeEx(Extern);/* free the Extern image node */
        free(str);
        return;
    }

    /* the binary printing for help! */
    /*printObjFile(str,head,IC,DC);*/

    /* Spread the address image node(head) with base64 in to object file */
    printObjFileBase64(str,head,IC,DC);

    if(Entry!=NULL){/*if the Entry node are not empty Spread the entry image node(Entry) with base10 in to entry file */
        printEntFile(str,Entry);
    }

    if(Extern!=NULL){/*if the Extern node are not empty Spread the extern image node(Extern) with base10 in to extern file */
        printExtFile(str,Extern);
    }

    freeListNode(head);/* free the addressing image node */
    freeSymbolTable(symbol);/* free the symbol table */
    freeListNodeEn(Entry);/* free the Entry image node */
    freeListNodeEx(Extern);/* free the Extern image node */
    free(str);/* free the file name */
}
