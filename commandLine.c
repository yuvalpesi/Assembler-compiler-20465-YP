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
    char *str=NULL;

    str= strDup(argv);
    str[strlen(str)]='\0';

    /* Spread the macros from the input file */
    if(!preProccesor(str)){
        printf("Couldn't spread the macros correctly into %s\n", str);
        freeListNode(head);
        freeSymbolTable(symbol);
        free(str);
        return;
    }

    if(firstPass(str,&head,symbol,&IC,&DC)!=True){
        printf("Couldn't compile the file %s\n", str);
        freeListNode(head);
        freeSymbolTable(symbol);
        free(str);
        return;
    }

    if(secPass(str,head,symbol,IC,DC)!=True){
        printf("Couldn't compile the file %s\n", str);
        freeListNode(head);
        freeSymbolTable(symbol);
        free(str);
        return;
    }

    /*printObjFile(str,head,IC,DC);*/

    printObjFileBase64(str,head,IC,DC);

    freeListNode(head);
    freeSymbolTable(symbol);
    free(str);
}

