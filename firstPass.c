#include "data.h"

int firstPass(char *argv, LineHolder **nodeHead, symbolTable *SymbolTable, int *IC, int*DC){
    FILE *fd=NULL;
    char *line=NULL,*sourceFile=NULL;
    int command,errorCounter=0,lineNumber=0;
    symbolTable *table=SymbolTable;
    LineHolder *head=*nodeHead; /* addressing linked list */
    char *function[FUNCTON_NUM]={ "mov","cmp","add","sub",
                                  "not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};

    sourceFile=(char*)malloc((strlen(argv)+5) * sizeof(char));
    line=(char*)calloc(MAX_LINE_LENGHT *4,sizeof(char));

    if(sourceFile == NULL || line==NULL){
        printf("Error: Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(sourceFile,argv);
    sourceFile=(char*)realloc(sourceFile,strlen(sourceFile)+4);
    if(sourceFile == NULL){
        printf("Error: Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    strcat(sourceFile,".am");

    if((fd=fopen(sourceFile,"r"))==NULL){
        printf("Error: cant open file %s\n",sourceFile);
        return False;
    }

    /*read the .am file and build the addressing linked list in the first pass.
     * and I check the line length in this function and set the code image and data image in one linked list. */
    while(!feof(fd) && fgets(line,MAX_LINE_LENGHT*4,fd)!=NULL){
        lineNumber++;/* line number counter */
        line[strlen(line)-1]='\0';

        /* see if the line length is not more then 80 */
        if(strlen(line)>MAX_LINE_LENGHT){
            /*memmove(line,line, strlen(line)*2);*/
            fprintf(stderr,"Error in file %s: Too long line '%s', can not pass 80 chars in line number %d \n",sourceFile,line,lineNumber);
            errorCounter++;
            continue;
        }

        /* see if there is label and save it in the table and if there is an error in the label return False */
        if(handleLabel(line,sourceFile,lineNumber,&errorCounter,*IC,*DC,table)==False){
            continue;
        }

        line= ignoreSpace(line);/* remove space before the first argument */

        switch(findDirectiveType(line)){
            case String:
                handleString(line,sourceFile,lineNumber,&errorCounter,DC,&head);
                continue;
            case Data:
                handleData(line,sourceFile,lineNumber,&errorCounter,DC,&head);
                continue;
            case Extern:
                handleExtern(line,sourceFile,lineNumber,&errorCounter,table);
                continue;
            case Entry:
                handleEntry(line,sourceFile,lineNumber,&errorCounter,table);
                continue;
            default:
                break;
        }

        command = commandType(line);/* find command type*/

        if(command!=ERROR){
            memmove(line,line+strlen(function[command]), strlen(line)+1);
            line= ignoreSpace(line);
            if(line[0]==','){ /* check for un necessary comma in the beginning of the operands*/
                fprintf(stderr,"Error in file %s: Illegal comma '%s' in line number %d \n",sourceFile,line,lineNumber);
                errorCounter++;
                continue;
            }
        }

        /* get inside to the right case of command and analyze the operands*/
        switch (command) {
            case mov:case cmp:case add:case lea:case sub:
                handleTowOperand(command,function[command],line,sourceFile,lineNumber,&errorCounter,IC,&head);
                break;
            case not:case clr:case inc:case dec:case jmp:case bne:case red:case prn:case jsr:
                handleOneOperand(command,function[command],line,sourceFile,lineNumber,&errorCounter,IC,&head);
                break;
            case rts:case stop:
                handleNoOperand(command,function[command],line,sourceFile,lineNumber,&errorCounter,IC,&head);
                break;
            default:
                fprintf(stderr,"Error in file %s: Undefined command name '%s' in line %d \n",sourceFile,line,lineNumber);
                errorCounter++;
                break;
        }
    }

    *nodeHead=head;
    fclose(fd);
    free(sourceFile);
    if(line!=NULL){
        free(line);
    }
    return errorCounter==0?True:False;
}

int commandType(char *str){
    int i=0,count=0;
    char *function[FUNCTON_NUM]={ "mov","cmp","add","sub",
                                  "not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};

    while(str[i]!='\0' && str[i]!=' ' && str[i]!='\t' && str[i]!='\n' && str[i]!=','){
        count++;
        i++;
    }

    for(i=0;i<FUNCTON_NUM;i++){
        if(strlen(function[i])==count && !strncmp(function[i],str,count)){
            return i;
        }
    }

    return ERROR;
}

int findDirectiveType(char *str){
    int i=0,count=0;
    char *DirectiveType[MAX_DIRECTIVE]={ ".string",".data",".extern",".entry"};

    while(str[i]!='\0' && str[i]!=' ' && str[i]!='\t' && str[i]!='\n' && str[i]!=','){
        count++;
        i++;
    }

    for(i=0;i<MAX_DIRECTIVE;i++){
        if(strlen(DirectiveType[i])==count && !strncmp(DirectiveType[i],str,count)){
            return i;
        }
    }

    return ERROR;
}

Operand *installBinary(char *name,unsigned int opcode,unsigned int source,unsigned int target,unsigned int ARE){
    int i;
    Operand *tmp=NULL;
    tmp= malloc(sizeof (Operand));
    if(tmp==NULL){
        printf("Error: Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    tmp->lableName= strDup(name);
    /* get inside to the right bit-filed directives and allocate memory and set all the info */
    if(findDirectiveType(name)!=ERROR){
        tmp->operandStucter.directiveSentence= malloc(sizeof (DirectiveSentence));

        if(tmp->operandStucter.directiveSentence==NULL){
            printf("Error: Failed to allocate memory.\n");
            exit(EXIT_FAILURE);
        }

        tmp->operandStucter.directiveSentence->directive=opcode;

    }else if((i= commandType(name))!=ERROR){
        tmp->operandStucter.first= malloc(sizeof (firstOp));

        if(tmp->operandStucter.first==NULL){
            printf("Error: Failed to allocate memory.\n");
            exit(EXIT_FAILURE);
        }

        if(i>=mov && i<=lea){
            tmp->operandStucter.first->ARE=ARE;
            tmp->operandStucter.first->targetOperand=target;
            tmp->operandStucter.first->opcode=i;
            tmp->operandStucter.first->sourceOperand=source;

        }
        if(i>=not && i<=jsr){
            tmp->operandStucter.first->ARE=ARE;
            tmp->operandStucter.first->targetOperand=target;
            tmp->operandStucter.first->opcode=i;
            tmp->operandStucter.first->sourceOperand=source;

        }
        if(i==rts || i==stop){
            tmp->operandStucter.first->ARE=ARE;
            tmp->operandStucter.first->targetOperand=target;
            tmp->operandStucter.first->opcode=i;
            tmp->operandStucter.first->sourceOperand=source;
        }
    } else if(allDigits(name)==True ){
        tmp->operandStucter.immediate= malloc(sizeof (DirectImmediate));

        if(tmp->operandStucter.immediate==NULL){
            printf("Error: Failed to allocate memory.\n");
            exit(EXIT_FAILURE);
        }

        tmp->operandStucter.immediate->ARE=ARE;
        tmp->operandStucter.immediate->Operand=opcode;
    } else if(checkRegister(name)!=ERROR){
        tmp->operandStucter.registerAddress= malloc(sizeof (registerDirectAddressing));

        if(tmp->operandStucter.registerAddress==NULL){
            printf("Error: Failed to allocate memory.\n");
            exit(EXIT_FAILURE);
        }

        tmp->operandStucter.registerAddress->ARE=ARE;
        tmp->operandStucter.registerAddress->FirstRed=target;
        tmp->operandStucter.registerAddress->SecRed=source;

    } else if(allDigits(name)!=True){
        tmp->operandStucter.address= malloc(sizeof (DirectAddress));

        if(tmp->operandStucter.address==NULL){
            printf("Error: Failed to allocate memory.\n");
            exit(EXIT_FAILURE);
        }

        tmp->operandStucter.address->ARE=ARE;
        tmp->operandStucter.address->Operand=opcode;
    }

    return tmp;
}

void freeBinary(Operand *temp){

    if(findDirectiveType(temp->lableName) != ERROR){
        free(temp->operandStucter.directiveSentence);

    }else if(commandType(temp->lableName)!=ERROR){
        free(temp->operandStucter.first);

    }else if(allDigits(temp->lableName)==True) {
        free(temp->operandStucter.immediate);

    }else if(checkRegister(temp->lableName)!=ERROR){
        free(temp->operandStucter.registerAddress);

    }else if(allDigits(temp->lableName)!=True){
        free(temp->operandStucter.address);
    }
}

int checkRegister(char *str){
    int i,count= strlen(str);
    char *registersName[REGISTER_NUM]={"@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7"};

    for(i=0;i<REGISTER_NUM;i++){
        if(strlen(registersName[i])==count && strncmp(registersName[i],str,count)==0){
            return i;
        }
    }

    return ERROR;
}

char *getLabelName(char *str){
    int i=0;
    char *temp=NULL;

    while(i< strlen(str)){

        if(str[i]==':'){
            temp=(char *) malloc(sizeof (char ) *(i+2));

            if(temp==NULL){
                printf("Error: Failed to allocate memory.\n");
                exit(EXIT_FAILURE);
            }

            strncpy(temp,str,i+1);
            temp[i+1]='\0';
            return temp;
        }
        if(str[i]==' ' || str[i+1]=='\0'){
            temp=(char *) malloc(sizeof (char ) *(i+2));

            if(temp==NULL){
                printf("Error: Failed to allocate memory.\n");
                exit(EXIT_FAILURE);
            }

            strncpy(temp,str,i+1);
            temp[i+1]='\0';
            return temp;
        }
        i++;
    }

    return NULL;
}

int checkLabelInCommand(char *str){
    int i;

    if(!isalpha(str[0])){
        return False;
    }

    for(i=1;i<MAX_LABLE;i++){
        if(!isalnum(str[i]) && i<strlen(str)){
            return False;
        }
    }

    return True;
}

int handleLabel(char *line, char *sourceFile,int lineNumber, int *errorCounter,int IC,int DC,symbolTable *table){
    char *tempToken=NULL,*labelName=NULL;

    labelName=getLabelName(line);/* copy the label if there is one in the line and save it in a holding argument */

    /* check for errors in the label */
    if(labelName[strlen(labelName)-1]==':' && (thereIsSpace(labelName)==True || !isalpha(labelName[0]))){
        fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,labelName,lineNumber);
        (*errorCounter)++;
        free(labelName);
        return False;
    }

    /* check the label and see if it's holding a .entry or .extern in the line if not get inside and add to symbol table */
    if(labelName[strlen(labelName)-1]==':'  && findDirectiveType(line+ strlen(labelName))!=Entry && findDirectiveType(line+ strlen(labelName))!=Extern){
        memmove(line,line+strlen(labelName), strlen(line)+1); /* remove the label from the line */
        line= ignoreSpace(line);
        tempToken= strDup(labelName);

        if(strlen(tempToken)>MAX_LABLE){
            fprintf(stderr,"Error in file %s: The '%s' Label too long can not pass 31 chars in line %d \n",sourceFile,tempToken,lineNumber);
            (*errorCounter)++;
            free(tempToken);
            free(labelName);
            return False;
        }

        tempToken[strlen(tempToken)-1]='\0'; /* remover the ':' */

        if(checkLabelInCommand(tempToken)==False){/* check if the label is valid without syntax errors*/
            fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,tempToken,lineNumber);
            (*errorCounter)++;
            free(tempToken);
            free(labelName);
            return False;
        }

        if(checkRegister(tempToken)!=ERROR || commandType(tempToken)!=ERROR){/* check if the label is valid not a command name or register */
            fprintf(stderr,"Error in file %s: The '%s' is not valid Label (assembly Reserved Words) in line %d \n",sourceFile,tempToken,lineNumber);
            (*errorCounter)++;
            free(tempToken);
            free(labelName);
            return False;
        }

        if(symbolTableLockUp(table,tempToken)!=NULL && symbolTableLockUpICDC(table,tempToken)!=ERROR){/* check if the label is already in the table ,and also it is a code or data image */
            fprintf(stderr,"Error in file %s: The '%s' is not valid Label (already use) in line %d \n",sourceFile,tempToken,lineNumber);
            (*errorCounter)++;
            free(tempToken);
            free(labelName);
            return False;
        }

        if(symbolTableLockUpType(table,tempToken)==sEXETRN){/* if after the label is an extern type and after the label it a variable it is error */
            fprintf(stderr,"Error in file %s: The '%s' is not valid Label can not define extern label before a local variable in line %d \n",sourceFile,tempToken,lineNumber);
            (*errorCounter)++;
            free(tempToken);
            free(labelName);
            return False;
        }

        if(commandType(line)!=ERROR){/* see if the label holding a command in the line if yes mark it with IC type*/
            symbolTableInsert(table,tempToken,IC,sIC);
        }else{
            symbolTableInsert(table,tempToken,DC,sDC);
        }
        free(tempToken);
    }

    /* check if after the label there is .entry or .extern and if yes ignore from it */
    if(labelName[strlen(labelName)-1]==':'  && (findDirectiveType(line+ strlen(labelName))==Entry || findDirectiveType(line+ strlen(labelName))==Extern)){
        memmove(line,line+strlen(labelName), strlen(line)+1);/* remove the label from the line */
    }

    free(labelName);
    return True;
}
