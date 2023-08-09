#include "data.h"

void handleExtern(char *Line, char *sourceFile, int lineNumber, int *errorCounter,symbolTable *table){
    char *secStr=NULL,*firsStr=NULL;
    char *line= strDup(Line);/* set a copy of Line */

    secStr= strDup(line);
    memmove(secStr,secStr+8, strlen(line)+1); /* save .extern form line */
    secStr= ignoreSpace(secStr);
    memmove(line,line+7, strlen(line)+1);/* remove the .extern from line */
    line= ignoreSpace(line);

    if(line[0]=='\0'){
        fprintf(stderr,"Error in file %s: Missing parameter un empty argument in line %d \n",sourceFile,lineNumber);
        (*errorCounter)++;
        free(line);
        free(secStr);
        return;
    }

    if(secStr[0]==','){
        fprintf(stderr,"Error in file %s: Illegal comma '%s' in line number %d \n",sourceFile,secStr,lineNumber);
        (*errorCounter)++;
        free(line);
        free(secStr);
        return;
    }

    if(secStr[strlen(secStr)-1]==','){
        fprintf(stderr,"Error in file %s: Illegal comma '%s' in line number %d \n",sourceFile,secStr,lineNumber);
        (*errorCounter)++;
        free(line);
        free(secStr);
        return;
    }
    /* check label by label in the line */
    while(secStr!=NULL) {
        firsStr = strFirstArgu(secStr);/* get the libel upto the comma or '\0' */
        if (firsStr == NULL){
            free(secStr);
            firsStr = NULL;
            break;
        }

        memmove(secStr, secStr + strlen(firsStr), strlen(secStr));/* remove the label from the line */
        secStr = ignoreSpace(secStr);

        if (secStr[0] == '\0') {
            free(secStr);
            secStr = NULL;
        }

        if (secStr != NULL && secStr[0] != ',') {
            (*errorCounter)++;
            fprintf(stderr, "Error in file %s: Missing comma '%s' in line %d \n", sourceFile, secStr, lineNumber);
            free(firsStr);
            break;
        }

        if (secStr != NULL && secStr[0] == ',') {
            memmove(secStr, secStr + 1, strlen(secStr));
            secStr = ignoreSpace(secStr);
        }

        if (checkLabelInCommand(firsStr) == False) {
            fprintf(stderr, "Error in file %s: The '%s' is not valid Label in line %d \n", sourceFile, firsStr,
                    lineNumber);
            (*errorCounter)++;
            free(firsStr);
            firsStr = NULL;
            continue;
        }

        if (symbolTableLockUpType(table, firsStr) == sENTRY || symbolTableLockUpType(table, firsStr) == sEXETRN) {
            fprintf(stderr, "Error in file %s: The '%s' is not valid Label (already use) in line %d \n", sourceFile,firsStr, lineNumber);
            (*errorCounter)++;
            free(firsStr);
            firsStr = NULL;
            continue;
        }
        /* add label into the symbol table with extern symbol */
        symbolTableInsert(table, firsStr, 0, sEXETRN);
        free(firsStr);
        firsStr = NULL;
    }
    free(secStr);
    free(line);
}

void handleEntry(char *Line, char *sourceFile, int lineNumber, int *errorCounter,symbolTable *table){
    char *secStr=NULL,*firsStr=NULL;
    char *line= strDup(Line);/* set a copy of Line */

    secStr= strDup(line);
    memmove(secStr,secStr+6, strlen(line)+1); /* save .extern form line */
    secStr= ignoreSpace(secStr);
    memmove(line,line+6, strlen(line)+1);/* remove the .extern from line */
    line= ignoreSpace(line);

    if(line[0]=='\0'){
        fprintf(stderr,"Error in file %s: Missing parameter un empty argument in line %d \n",sourceFile,lineNumber);
        (*errorCounter)++;
        free(line);
        free(secStr);
        return;
    }

    if(secStr[0]==','){
        fprintf(stderr,"Error in file %s: Illegal comma '%s' in line number %d \n",sourceFile,secStr,lineNumber);
        (*errorCounter)++;
        free(line);
        free(secStr);
        return;
    }

    if(secStr[strlen(secStr)-1]==','){
        fprintf(stderr,"Error in file %s: Illegal comma '%s' in line number %d \n",sourceFile,secStr,lineNumber);
        (*errorCounter)++;
        free(line);
        free(secStr);
        return;
    }
    /* check label by label in the line */
    while(secStr!=NULL) {
        firsStr = strFirstArgu(secStr);/* get the libel upto the comma or '\0' */
        if (firsStr == NULL){
            free(secStr);
            firsStr = NULL;
            break;
        }

        memmove(secStr, secStr + strlen(firsStr), strlen(secStr));/* remove the label from the line */
        secStr = ignoreSpace(secStr);

        if (secStr[0] == '\0') {
            free(secStr);
            secStr = NULL;
        }

        if (secStr != NULL && secStr[0] != ',') {
            (*errorCounter)++;
            fprintf(stderr, "Error in file %s: Missing comma '%s' in line %d \n", sourceFile, secStr, lineNumber);
            free(firsStr);
            break;
        }

        if (secStr != NULL && secStr[0] == ',') {
            memmove(secStr, secStr + 1, strlen(secStr));
            secStr = ignoreSpace(secStr);
        }

        if (checkLabelInCommand(firsStr) == False) {
            fprintf(stderr, "Error in file %s: The '%s' is not valid Label in line %d \n", sourceFile, firsStr,
                    lineNumber);
            (*errorCounter)++;
            free(firsStr);
            firsStr = NULL;
            continue;
        }

        if (symbolTableLockUpType(table, firsStr) == sENTRY || symbolTableLockUpType(table, firsStr) == sEXETRN) {
            fprintf(stderr, "Error in file %s: The '%s' is not valid Label (already use) in line %d \n", sourceFile,firsStr, lineNumber);
            (*errorCounter)++;
            free(firsStr);
            firsStr = NULL;
            continue;
        }
        /* add label into the symbol table with extern symbol */
        symbolTableInsert(table, firsStr, 0, sENTRY);
        free(firsStr);
        firsStr = NULL;
    }
    free(secStr);
    free(line);
}

void handleString(char *Line, char *sourceFile, int lineNumber, int *errorCounter,int *DC,LineHolder **head){
    Operand *codeNum=NULL;
    char *secStr=NULL;
    int c,index;
    char *line= strDup(Line);/* set a copy of Line */

    secStr= strDup(line);/* copy the .string from line*/
    secStr[8]='\0';
    memmove(line,line+7, strlen(line)+1);/* remove the .string from line*/
    line= ignoreSpace(line);

    if(line[0]=='\0'){
        fprintf(stderr,"Error in file %s: Missing parameter un empty argument in line %d \n",sourceFile,lineNumber);
        free(secStr);
        free(line);
        (*errorCounter)++;
        return;
    }

    if(line[0]!='"' || line[strlen(line)-1]!='"'){
        fprintf(stderr,"Error in file %s: '%s' is not valid string (missing quotation marks) in line %d \n",sourceFile,line,lineNumber);
        free(secStr);
        free(line);
        (*errorCounter)++;
        return;
    }
    RemoveQuotationMarks(line);
    index=0;
    if(line[index]=='\0'){/* if there is no char between the quotation marks and only the '\0' to the addressing node */
        codeNum= installBinary(secStr,'\0',0,0,0);
        addNode(head, createNodeItem(*DC, codeNum));
        (*DC)++;
    }

    while (line[index] != '\0'){/* check char by char and add it to the addressing node */
        c=(unsigned char)line[index];
        /* crate the binary operand for the data image */
        codeNum= installBinary(secStr,c,0,0,0);
        addNode(head, createNodeItem(*DC, codeNum));/* add and crate the linked node (addressing image linked list node)*/
        index++;
        (*DC)++;
        if (line[index] == '\0'){ /* include the null terminator at the end of the string*/
            codeNum= installBinary(secStr,'\0',0,0,0);
            addNode(head, createNodeItem(*DC, codeNum));
            (*DC)++;
        }
    }

    free(line);
    free(secStr);
}

void handleData(char *Line, char *sourceFile, int lineNumber, int *errorCounter,int *DC,LineHolder **head){
    Operand *codeNum=NULL;
    char *secStr=NULL;
    int number,index;
    char *line= strDup(Line);/* set a copy of Line */

    secStr= strDup(line);/* save the .data label from the line */
    secStr[6]='\0';
    memmove(line,line+5, strlen(line)+1);/* remove the .data from line */
    line= ignoreSpace(line);

    if(line[0]=='\0'){
        fprintf(stderr,"Error in file %s: Missing parameter un empty argument in line %d \n",sourceFile,lineNumber);
        (*errorCounter)++;
        free(line);
        free(secStr);
        return;
    }

    if(findComma(line)==False){ /* find if there is comma between 2 argument or there is illegal space */
        fprintf(stderr,"Error in file %s: Missing comma between the arguments or there is illegal space '%s' line number %d \n",sourceFile,line,lineNumber);
        (*errorCounter)++;
        free(line);
        free(secStr);
        return;
    }

    RemoveAllSpaces(line);
    index = 0;

    if(line[index]==','){
        fprintf(stderr,"Error in file %s: Illegal comma '%s' in line number %d \n",sourceFile,line,lineNumber);
        (*errorCounter)++;
        free(line);
        free(secStr);
        return;
    }
    /* check the numbers one by one upto the '\0' */
    while (line[index] != '\0' && index<strlen(line)) {

        if(!isdigit(line[index]) && line[index]!=',' && line[index]!='+' && line[index]!='\0'){
            if(!line[index]){
                fprintf(stderr,"Error in file %s: Missing parameter in line %d \n",sourceFile,lineNumber);
                (*errorCounter)++;
                index++;
                continue;
            }
        }

        if(line[index+1]=='\0'&& !isdigit(line[index])){
            fprintf(stderr,"Error in file %s: Extraneous text after end of command '%s' in line %d \n",sourceFile,line,lineNumber);
            (*errorCounter)++;
            index++;
            continue;
        }

        if(line[index] != ',' && line[index] != '\0'){
            if(allDigitsForData(line)!=ERROR){/* check if the number is valid */
                index+=allDigitsForData(line);
                fprintf(stderr,"Error in file %s: Invalid parameter - not a number '%s' in line number %d \n",sourceFile,line,lineNumber);
                (*errorCounter)++;
                while (line[index]!=',')index++;
                index++;
                memmove(line,line+index, strlen(line));
                continue;
            }
            number=getNumberFromData(line,&index);/* get the number from the line upto comma or '\0'*/
            if(!isdigit(line[index-1]) && line[index]=='\0'){
                fprintf(stderr,"Error in file %s: Extraneous text after end of command '%s' in line %d \n",sourceFile,line,lineNumber);
                (*errorCounter)++;
                break;
            }

            if(line[index-1]==',' && line[index]==','){
                fprintf(stderr,"Error in file %s: Multiple consecutive commas '%s' in line number %d \n",sourceFile,line,lineNumber);
                (*errorCounter)++;
                break;
            }
            memmove(line,line+index, strlen(line));/* remove the comma from the line */
            index=0;


            if(number>MAX_NUMBER_SIZE || number<LOW_NUMBER_SIZE){
                if(number<0){
                    fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too small\n",sourceFile,lineNumber,number);
                }else{
                    fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too big\n",sourceFile,lineNumber,number);
                }
                index++;
                (*errorCounter)++;
                continue;
            }
            /* crate the binary operand for the data image */
            codeNum= installBinary(secStr,number,0,0,0);
            addNode(head, createNodeItem(*DC, codeNum));/* add and crate the linked node (addressing image linked list node)*/
            (*DC)++;

            continue;
        }
        index++;
    }

    free(secStr);
    free(line);
}
