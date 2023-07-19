#include "data.h"

int firstPass(char *argv,LineHolder **nodeHead,symbolTable *symbolTabl,int *IC,int*DC){
    FILE *fd=NULL;
    char *line=NULL,*sourceFile=NULL,*tempToken=NULL,*secStr=NULL,*firsStr=NULL,*labelname=NULL;
    int command,number,c,strSpace,index=0,errorCounter=0,lineNumber=0;
    symbolTable *table=symbolTabl;
    LineHolder *head=*nodeHead;
    Operand *codeNum=NULL;
    lineStr *linestr=NULL,*temp=NULL;
    char *function[FUNCTON_NUM]={ "mov","cmp","add","sub",
                                  "not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};

    sourceFile=(char*)malloc((strlen(argv)+4) * sizeof(char));
    line=(char*)calloc(MAX_LINE_LENGHT *4,sizeof(char));

    if(sourceFile == NULL || line==NULL){
        printf("Error: Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    strncpy(sourceFile,argv,strlen(argv));
    sourceFile[strlen(argv)]='\0';
    strcat(sourceFile,".am");
    sourceFile[strlen(sourceFile)]='\0';


    if((fd=fopen(sourceFile,"r"))==NULL){
        printf("Error: cant open file %s\n",sourceFile);
        return False;
    }

    while (!feof(fd) && fgets(line,MAX_LINE_LENGHT*4,fd)!=NULL){
        addNodeLine(&linestr, createNodeLine(line));
    }

    free(line);
    line=NULL;
    temp=linestr;

    while(linestr!=NULL){
        lineNumber++;

        if(firsStr!=NULL){
            free(firsStr);
            firsStr=NULL;
        }

        if(line!=NULL){
            free(line);
            line=NULL;
        }

        line=strDup(linestr->lineStr);
        if(strlen(line)>MAX_LINE_LENGHT+2){
            memmove(line,line, strlen(line)*2);
            line[strlen(line)-1]='\0';
            fprintf(stderr,"Error in file %s: Too long line '%s', can not pass 80 chars in line number %d \n",sourceFile,line,lineNumber);
            errorCounter++;
            free(line);
            line=NULL;
            linestr=linestr->next;
            continue;
        }
        labelname=labelName(line);

        if(labelname[strlen(labelname)-1]==':' && (thereIsSpace(labelname)==True || !isalpha(labelname[0]))){
            fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,labelname,lineNumber);
            errorCounter++;
            free(labelname);
            linestr=linestr->next;
            continue;
        }

        if(labelname[strlen(labelname)-1]==':'  && checksEntry(line+ strlen(labelname))!=True && checksExtern(line+ strlen(labelname))!=True){
            strSpace = strlen(labelname);
            memmove(line,line+strSpace, strlen(line)+1);
            line=ignorSpace(line);

            tempToken=(char *) malloc(strSpace*sizeof (char));
            if(tempToken==NULL){
                printf("Error: Failed to allocate memory.\n");
                exit(1);
            }

            strcpy(tempToken,labelname);

            if(strlen(tempToken)>MAX_LABLE){
                fprintf(stderr,"Error in file %s: The '%s' Label too long can not pass 31 chars in line %d \n",sourceFile,tempToken,lineNumber);
                errorCounter++;
                free(tempToken);
                linestr=linestr->next;
                continue;
            }

            tempToken[strlen(tempToken)-1]='\0'; /* remover the ':' */

            if(checkLabelInCommand(tempToken)==False){
                fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,tempToken,lineNumber);
                errorCounter++;
                free(tempToken);
                linestr=linestr->next;
                continue;
            }

            if(checkRegister(tempToken)!=ERROR || commandType(tempToken)!=ERROR){
                fprintf(stderr,"Error in file %s: The '%s' is not valid Label (assembly Reserved Words) in line %d \n",sourceFile,tempToken,lineNumber);
                errorCounter++;
                free(tempToken);
                linestr=linestr->next;
                continue;
            }

            if(symbolTableLockUp(table,tempToken)!=NULL && symbolTableLockUpICDC(table,tempToken)!=ERROR){
                fprintf(stderr,"Error in file %s: The '%s' is not valid Label (already use) in line %d \n",sourceFile,tempToken,lineNumber);
                errorCounter++;
                free(tempToken);
                linestr=linestr->next;
                continue;
            }

            if(commandType(line)!=ERROR){
                symbolTableInsert(table,tempToken,*IC,sIC);
                free(tempToken);
            } else{
                symbolTableInsert(table,tempToken,*DC,sDC);
                free(tempToken);
            }
        }

        if(labelname[strlen(labelname)-1]==':'  && (checksEntry(line+ strlen(labelname))==True ||checksExtern(line+ strlen(labelname))==True)){
            strSpace = strlen(labelname);
            memmove(line,line+strSpace, strlen(line)+1);
            line=ignorSpace(line);
            free(labelname);
            labelname=NULL;
        }

        if(labelname!=NULL){
            free(labelname);
        }

        if((checkData(line) == True || checkString(line) == True )) {
            if (checkString(line) == True ) {
                secStr= strDup(line);
                secStr[8]='\0';
                memmove(line,line+7, strlen(line)+1);
                line= ignorSpace(line);

                if(line[0]=='\n'){
                    line[strlen(line)-1]='\0';
                    fprintf(stderr,"Error in file %s: Missing parameter un empty argument in line %d \n",sourceFile,lineNumber);
                    errorCounter++;
                    linestr=linestr->next;
                    continue;
                }

                if(line[strlen(line)-1]=='\n'){
                    line[strlen(line)-1]='\0';
                }
                if(line[0]!='"' || line[strlen(line)-1]!='"'){
                    fprintf(stderr,"Error in file %s: '%s' is not valid string (missing quotation marks) in line %d \n",sourceFile,line,lineNumber);
                    errorCounter++;
                    linestr=linestr->next;
                    continue;
                }
                RemoveQuotationMarks(line);
                index=0;
                while (line[index] != '\0'){
                    c=(unsigned char)line[index];
                    codeNum= instalBinary(secStr,c,0,0,0);
                    addNode(&head, createNodeItem(*DC, codeNum));
                    index++;
                    (*DC)++;
                    if (line[index] == '\0'){
                        codeNum= instalBinary(secStr,'\0',0,0,0);
                        addNode(&head, createNodeItem(*DC, codeNum));
                        (*DC)++;
                    }
                }
                linestr=linestr->next;
                free(secStr);
                continue;
            } else if (checkData(line) == True) {
                secStr= strDup(line);
                secStr[6]='\0';
                memmove(line,line+5, strlen(line)+1);
                line= ignorSpace(line);

                if(line[0]=='\n'){
                    line[strlen(line)-1]='\0';
                    fprintf(stderr,"Error in file %s: Missing parameter un empty argument in line %d \n",sourceFile,lineNumber);
                    errorCounter++;
                    linestr=linestr->next;
                    continue;
                }

                if(findComma(line)==False){
                    line[strlen(line)-1]='\0';
                    fprintf(stderr,"Error in file %s: Missing comma between the arguments or there is illegal space '%s' line number %d \n",sourceFile,line,lineNumber);
                    errorCounter++;
                    linestr=linestr->next;
                    continue;
                }

                RemoveAllSpaces(line);
                index = 0;

                if(line[index]==','){
                    fprintf(stderr,"Error in file %s: Illegal comma '%s' in line number %d \n",sourceFile,line,lineNumber);
                    errorCounter++;
                    linestr=linestr->next;
                    continue;
                }

                while (line[index] != '\0' && index<strlen(line)) {

                    if(!isdigit(line[index]) && line[index]!=',' && line[index]!='+' && line[index]!='\0'){
                        if(!line[index]){
                            fprintf(stderr,"Error in file %s: Missing parameter in line %d \n",sourceFile,lineNumber);
                            errorCounter++;
                            index++;
                            continue;
                        }
                    }

                    if(line[index+1]=='\0'&& !isdigit(line[index])){
                        fprintf(stderr,"Error in file %s: Extraneous text after end of command '%s' in line %d \n",sourceFile,line,lineNumber);
                        errorCounter++;
                        index++;
                        continue;
                    }

                    if(line[index] != ',' && line[index] != '\0'){
                        if(allDigitsForData(line)!=ERROR){
                            index+=allDigitsForData(line);
                            fprintf(stderr,"Error in file %s: Invalid parameter - not a number '%s' in line number %d \n",sourceFile,line,lineNumber);
                            errorCounter++;
                            while (line[index]!=',')index++;
                            index++;
                            memmove(line,line+index, strlen(line));
                            continue;
                        }
                        number=getNumberFromData(line,&index);
                        if(!isdigit(line[index-1]) && line[index]=='\0'){
                            fprintf(stderr,"Error in file %s: Extraneous text after end of command '%s' in line %d \n",sourceFile,line,lineNumber);
                            errorCounter++;
                            break;
                        }

                        if(line[index-1]==',' && line[index]==','){
                            fprintf(stderr,"Error in file %s: Multiple consecutive commas '%s' in line number %d \n",sourceFile,line,lineNumber);
                            errorCounter++;
                            break;
                        }
                        memmove(line,line+index, strlen(line));
                        index=0;


                        if(number>MAX_NUMBER_SIZE || number<LOW_NUMBER_SIZE){
                            if(number<0){
                                fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too small\n",sourceFile,lineNumber,number);
                            }else{
                                fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too big\n",sourceFile,lineNumber,number);
                            }
                            index++;
                            errorCounter++;
                            continue;
                        }

                        codeNum= instalBinary(secStr,number,0,0,0);
                        addNode(&head, createNodeItem(*DC, codeNum));
                        (*DC)++;

                        continue;
                    }

                    index++;
                }


                linestr=linestr->next;
                free(secStr);
                continue;
            }
        } else if ((checksExtern(line)==True || checksEntry(line)==True) ) {
            if (checksExtern(line)==True ){
                secStr= strDup(line);
                memmove(secStr,secStr+8, strlen(line)+1);
                secStr= ignorSpace(secStr);
                memmove(line,line+7, strlen(line)+1);
                line= ignorSpace(line);

                if(line[0]=='\n'){
                    line[strlen(line)-1]='\0';
                    fprintf(stderr,"Error in file %s: Missing parameter un empty argument in line %d \n",sourceFile,lineNumber);
                    errorCounter++;
                    linestr=linestr->next;
                    continue;
                }

                if(secStr[strlen(secStr)-1]=='\n'){
                    secStr[strlen(secStr)-1]='\0';
                }
                if(firsStr!=NULL)free(firsStr);

                if(secStr[0]==','){
                    fprintf(stderr,"Error in file %s: Illegal comma '%s' in line number %d \n",sourceFile,secStr,lineNumber);
                    errorCounter++;
                    linestr=linestr->next;
                    continue;
                }

                if(secStr[strlen(secStr)-1]==','){
                    fprintf(stderr,"Error in file %s: Illegal comma '%s' in line number %d \n",sourceFile,secStr,lineNumber);
                    errorCounter++;
                    linestr=linestr->next;
                    continue;
                }

                while(secStr!=NULL){
                    firsStr=strFirstArgu(secStr);
                    if(firsStr==NULL){
                        free(secStr);
                        firsStr=NULL;
                        break;
                    }

                    memmove(secStr,secStr+strlen(firsStr), strlen(secStr));
                    secStr= ignorSpace(secStr);

                    if(secStr[0]=='\0'){
                        free(secStr);
                        secStr=NULL;
                    }

                    if(secStr!=NULL && secStr[0]!=','){
                        errorCounter++;
                        fprintf(stderr,"Error in file %s: Missing comma '%s' in line %d \n",sourceFile,secStr,lineNumber);
                        break;
                    }

                    if(secStr!=NULL && secStr[0]==',' ){
                        memmove(secStr,secStr+1, strlen(secStr));
                        secStr= ignorSpace(secStr);
                    }

                    if(checkLabelInCommand(firsStr)==False){
                        fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,firsStr,lineNumber);
                        errorCounter++;
                        free(firsStr);
                        firsStr=NULL;
                        continue;
                    }

                    if(symbolTableLockUpType(table,firsStr)==sENTRY || symbolTableLockUpType(table,firsStr)==sEXETRN){
                        fprintf(stderr,"Error in file %s: The '%s' is not valid Label (already use) in line %d \n",sourceFile,firsStr,lineNumber);
                        errorCounter++;
                        free(firsStr);
                        firsStr=NULL;
                        continue;
                    }

                    symbolTableInsert(table,firsStr,0,sEXETRN);
                    free(firsStr);
                    firsStr=NULL;
                }

                linestr=linestr->next;
                continue;
            } else if(checksEntry(line)==True){
                secStr=strDup(line);
                memmove(secStr,secStr+7, strlen(line)+1);

                memmove(line,line+6, strlen(line)+1);
                line= ignorSpace(line);

                if(line[0]=='\n'){
                    line[strlen(line)-1]='\0';
                    fprintf(stderr,"Error in file %s: Missing parameter un empty argument in line %d \n",sourceFile,lineNumber);
                    errorCounter++;
                    linestr=linestr->next;
                    continue;
                }

                if(secStr[strlen(secStr)-1]=='\n'){
                    secStr[strlen(secStr)-1]='\0';
                }

                if(thereIsSpace(secStr)==True){
                    fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,secStr,lineNumber);
                    errorCounter++;
                    free(secStr);
                    linestr=linestr->next;
                    continue;
                }

                RemoveAllSpaces(secStr);

                if(checkLabelInCommand(secStr)==False){
                    fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,secStr,lineNumber);
                    errorCounter++;
                    free(secStr);
                    linestr=linestr->next;
                    continue;
                }

                if((symbolTableLockUpType(table,secStr)==sENTRY || symbolTableLockUpType(table,secStr)==sEXETRN)){
                    fprintf(stderr,"Error in file %s: The '%s' is not valid Label (already use) in line %d \n",sourceFile,secStr,lineNumber);
                    errorCounter++;
                    free(secStr);
                    linestr=linestr->next;
                    continue;
                }

                symbolTableInsert(table,secStr,0,sENTRY);
                linestr=linestr->next;
                free(secStr);
                continue;
            }
        }

        line=ignorSpace(line);
        command = commandType(line);

        if(firsStr!=NULL){
            free(firsStr);
            firsStr=NULL;
        }
        if (command != ERROR) {
            memmove(line,line+strlen(function[command]), strlen(line)+1);
            line= ignorSpace(line);

            if(line[0]==','){
                line[strlen(line)-1]='\0';
                fprintf(stderr,"Error in file %s: Illegal comma '%s' in line number %d \n",sourceFile,line,lineNumber);
                errorCounter++;
                linestr=linestr->next;
                continue;
            }
            switch (command) {
                case mov:case cmp:case add:case lea:case sub:
                    firsStr=strFirstArgu(line);

                    if(!firsStr[0]){
                        fprintf(stderr,"Error in file %s: Missing parameter in line %d \n",sourceFile,lineNumber);
                        errorCounter++;
                        break;
                    }

                    memmove(line,line+(strlen(firsStr)), strlen(line)+1); /*sec argu*/
                    if(firsStr[strlen(firsStr)-1]==' '){
                        firsStr[strlen(firsStr)-1]='\0';
                    }
                    line= ignorSpace(line);

                    if(line[0]!=','){
                        fprintf(stderr,"Error in file %s: Missing comma '%s' in line %d \n",sourceFile,line,lineNumber);
                        errorCounter++;
                        break;
                    }

                    memmove(line,line+1, strlen(line)+1);
                    if(line[strlen(line)-1]=='\n'){
                        line[strlen(line)-1]='\0';
                    }

                    if(!line[0]){
                        fprintf(stderr,"Error in file %s: Missing parameter in line %d \n",sourceFile,lineNumber);
                        errorCounter++;
                        break;
                    }

                    if(line[0]==','){
                        fprintf(stderr,"Error in file %s: Multiple consecutive commas '%s' in line %d \n",sourceFile,line,lineNumber);
                        errorCounter++;
                        break;
                    }

                    line= ignorSpace(line);

                    if(thereIsSpace(line)==True || thereIsSpace(firsStr)==True){
                        if(thereIsSpace(firsStr)==True){
                            fprintf(stderr,"Error in file %s: The '%s' is not valid parameter can not be with spaces in line %d \n",sourceFile,firsStr,lineNumber);
                            errorCounter++;
                            break;
                        } else{
                            fprintf(stderr,"Error in file %s: Extraneous text after end of command '%s' in line %d \n",sourceFile,line,lineNumber);
                            errorCounter++;
                            break;
                        }
                    }

                    if(line[strlen(line)-1]==','){
                        fprintf(stderr,"Error in file %s: Extraneous text after end of command '%s' in line %d \n",sourceFile,line,lineNumber);
                        errorCounter++;
                        break;
                    }

                    if(firsStr[0]=='@'){
                        if(checkRegister(firsStr)==ERROR){
                            errorCounter++;
                            fprintf(stderr,"Error in file %s: Undefined register number name '%s' in line %d \n",sourceFile,firsStr,lineNumber);
                            break;
                        }
                    }

                    if(line[0]=='@'){
                        if(checkRegister(line)==ERROR){
                            errorCounter++;
                            fprintf(stderr,"Error in file %s: Undefined register number name '%s' in line %d \n",sourceFile,line,lineNumber);
                            break;
                        }
                    }

                    if(command!=lea && checkRegister(firsStr)!= ERROR && checkRegister(line)!=ERROR){

                        if(checkRegister(firsStr)!=ERROR && checkRegister(line)!=ERROR){
                            codeNum=instalBinary(function[command],command,registerDirectAddressing,registerDirectAddressing,A);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                        }

                        if(checkRegister(firsStr)!=ERROR && checkRegister(line)!=ERROR){
                            codeNum=instalBinary(firsStr,0,checkRegister(firsStr),checkRegister(line),A);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                        }

                    } else if(command==lea){ /* lea target   */

                        if(allDigits(firsStr)==True || checkRegister(firsStr)!=ERROR){
                            errorCounter++;
                            fprintf(stderr,"Error in file %s: Undefined operand type name '%s' in line %d \n",sourceFile,firsStr,lineNumber);
                            break;
                        }

                        if(checkRegister(line)!=ERROR){
                            if(allDigits(firsStr)!=True){
                               codeNum=instalBinary(function[command],command,directAddressing,registerDirectAddressing,A);
                               addNode(&head, createNodeItem(*IC, codeNum));
                               (*IC)++;
                            }

                        } else if(allDigits(firsStr)!=True && allDigits(line)!=True){
                            if(checkLabelInCommand(firsStr)!=True){
                                fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,firsStr,lineNumber);
                                errorCounter++;
                                break;
                            }
                            if(checkLabelInCommand(line)!=True){
                                fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,line,lineNumber);
                                errorCounter++;
                                break;
                            }
                            codeNum=instalBinary(function[command],command,directAddressing, directAddressing,A);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                        }


                        /* first argu */
                        codeNum=instalBinary(firsStr,0,0,0,R);
                        addNode(&head, createNodeItem(*IC, codeNum));
                        (*IC)++;

                        /* sec argu */
                        if(checkRegister(line)!=ERROR){
                            number=checkRegister(line);
                            codeNum=instalBinary(line,0,0,number,A);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                        } else if(allDigits(line)!=True){
                            codeNum=instalBinary(line,0,0,0,R);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                        }

                    } else { /* all the rest*/

                        if(command!=cmp && allDigits(line)==True){
                            errorCounter++;
                            fprintf(stderr,"Error in file %s: Undefined operand type name '%s' in line %d \n",sourceFile,line,lineNumber);
                            break;
                        }

                        if(checkRegister(firsStr)!=ERROR || checkRegister(line)!=ERROR){
                            if(checkRegister(firsStr)!=ERROR){
                                if(allDigits(line)!=True){
                                    codeNum=instalBinary(function[command],command,registerDirectAddressing,directAddressing,A);
                                    addNode(&head, createNodeItem(*IC, codeNum));
                                    (*IC)++;
                                }

                                if(allDigits(line)==True && command==cmp){
                                    codeNum=instalBinary(function[command],command,registerDirectAddressing,immediateAddressing,A);
                                    addNode(&head, createNodeItem(*IC, codeNum));
                                    (*IC)++;
                                }

                            }else if(checkRegister(line)!=ERROR){
                                if(allDigits(firsStr)==True){
                                    codeNum=instalBinary(function[command],command,immediateAddressing,registerDirectAddressing,A);
                                    addNode(&head, createNodeItem(*IC, codeNum));
                                    (*IC)++;
                                }

                                if(allDigits(firsStr)!=True){
                                    codeNum=instalBinary(function[command],command,directAddressing,registerDirectAddressing,A);
                                    addNode(&head, createNodeItem(*IC, codeNum));
                                    (*IC)++;
                                }
                            }

                        }else if(allDigits(firsStr)==True && checkRegister(firsStr)==ERROR && checkRegister(line)==ERROR){
                            if(allDigits(line)!=True){
                                codeNum=instalBinary(function[command],command,immediateAddressing,directAddressing,A);
                                addNode(&head, createNodeItem(*IC, codeNum));
                                (*IC)++;
                            }

                            if(allDigits(line)==True && command==cmp){
                                codeNum=instalBinary(function[command],command,immediateAddressing,immediateAddressing,A);
                                addNode(&head, createNodeItem(*IC, codeNum));
                                (*IC)++;
                            }

                        }else if(allDigits(firsStr)!=True && checkRegister(firsStr)==ERROR && checkRegister(line)==ERROR){
                            if(allDigits(line)!=True) {
                                codeNum = instalBinary( function[command], command, directAddressing,directAddressing,A);
                                addNode(&head, createNodeItem(*IC, codeNum));
                                (*IC)++;
                            }

                            if(allDigits(line)==True && command==cmp){
                                codeNum=instalBinary(function[command],command,directAddressing,immediateAddressing,A);
                                addNode(&head, createNodeItem(*IC, codeNum));
                                (*IC)++;
                            }
                        }

                        /* first argu */
                        if(allDigits(firsStr)==True){
                            number= atoi(firsStr);

                            if(number>WOLD_NUMBER_MAX_SIZE || number<WOLD_NUMBER_LOW_SIZE){
                                if(number<0){
                                    fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too small\n",sourceFile,lineNumber,number);
                                }else{
                                    fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too big\n",sourceFile,lineNumber,number);
                                }
                                index++;
                                errorCounter++;
                                break;
                            }

                            codeNum=instalBinary(firsStr,number,0,0,A);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                        } else if(checkRegister(firsStr)!=ERROR){
                            number= checkRegister(firsStr);
                            codeNum=instalBinary(firsStr,0,number,0,A);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                        } else {
                            if(checkLabelInCommand(firsStr)!=True){
                                fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,firsStr,lineNumber);
                                errorCounter++;
                                break;
                            }

                            codeNum=instalBinary(firsStr,0,0,0,R);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                        }

                        /* sec argu */
                         if(checkRegister(line)!=ERROR){
                             number=checkRegister(line);
                             codeNum=instalBinary(line,0,0,number,A);
                             addNode(&head, createNodeItem(*IC, codeNum));
                             (*IC)++;
                        } else if(allDigits(line)!=True){
                             if(checkLabelInCommand(line)!=True){
                                 fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,line,lineNumber);
                                 errorCounter++;
                                 break;
                             }

                            codeNum=instalBinary(line,0,0,0,R);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                        } else if(command==cmp && allDigits(line)==True){
                             number= atoi(firsStr);

                             if(number>WOLD_NUMBER_MAX_SIZE || number<WOLD_NUMBER_LOW_SIZE){
                                 if(number<0){
                                     fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too small\n",sourceFile,lineNumber,number);
                                 }else{
                                     fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too big\n",sourceFile,lineNumber,number);
                                 }
                                 index++;
                                 errorCounter++;
                                 break;
                             }

                             codeNum=instalBinary(line,number,0,0,A);
                             addNode(&head, createNodeItem(*IC, codeNum));
                             (*IC)++;
                         }
                    }
                    free(firsStr);
                    firsStr=NULL;
                    break;

                case not:case clr:case inc:case dec:case jmp:case bne:case red:case prn:case jsr:
                    firsStr= strFirstArgu(line);
                    memmove(line,line+ strlen(firsStr), strlen(line));

                    if(line[0]=='\0'){
                        free(line);
                        line=NULL;
                    }
                    line= ignorSpace(line);

                    if(line!=NULL && line[strlen(line)-1]=='\n'){
                        line[strlen(line)-1]='\0';
                    }

                    if( line!=NULL && (line[0]==',' || thereIsSpace(line)|| findComma(line)==True || allDigits(line) ||
                            allDigits(line)!=True)){
                        fprintf(stderr,"Error in file %s: Extraneous text after end of command '%s' in line number %d \n",sourceFile,line,lineNumber);
                        errorCounter++;
                        break;
                    }

                    line= strDup(firsStr);
                    free(firsStr);
                    firsStr=NULL;

                    if(line[strlen(line)-1]=='\n'){
                        line[strlen(line)-1]='\0';
                    }

                    if(!line[0]){
                        fprintf(stderr,"Error in file %s: Missing parameter in line %d \n",sourceFile,lineNumber);
                        errorCounter++;
                        break;
                    }

                    if(line[0]=='@'){
                        if(checkRegister(line)==ERROR){
                            errorCounter++;
                            fprintf(stderr,"Error in file %s: Undefined register number name '%s' in line %d \n",sourceFile,line,lineNumber);
                            break;
                        }
                    }

                    if(command!=prn && allDigits(line)==True){
                        errorCounter++;
                        fprintf(stderr,"Error in file %s: Undefined operand type name '%s' in line %d \n",sourceFile,line,lineNumber);
                        break;
                    }

                    if(command==prn){

                        if(allDigits(line)==True){
                            codeNum=instalBinary(function[command],command,0,immediateAddressing,A);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                            number=atoi(line);

                            if(number>WOLD_NUMBER_MAX_SIZE || number<WOLD_NUMBER_LOW_SIZE){
                                if(number<0){
                                    fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too small\n",sourceFile,lineNumber,number);
                                }else{
                                    fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too big\n",sourceFile,lineNumber,number);
                                }
                                errorCounter++;
                                break;
                            }

                            codeNum=instalBinary(line,number,0,0,A);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;

                        } else if(checkRegister(line)!=ERROR){
                            codeNum=instalBinary(function[command],command,0,registerDirectAddressing,A);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                            codeNum=instalBinary(line,0,0,checkRegister(line),A);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;

                        } else{

                            if(checkLabelInCommand(line)!=True){
                                fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,line,lineNumber);
                                errorCounter++;
                                break;
                            }

                            codeNum=instalBinary(function[command],command,0,directAddressing,A);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;

                            codeNum=instalBinary(line,0,0,0,R);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                        }

                    }else{

                         if(checkRegister(line)!=ERROR){
                            codeNum=instalBinary(function[command],command,0,registerDirectAddressing,A);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                            codeNum=instalBinary(line,0,0,checkRegister(line),A);
                             addNode(&head, createNodeItem(*IC, codeNum));
                             (*IC)++;

                        } else if(allDigits(line)!=True){

                             if(checkLabelInCommand(line)!=True){
                                 fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,line,lineNumber);
                                 errorCounter++;
                                 break;
                             }

                            codeNum=instalBinary(function[command],command,0,directAddressing,A);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                            codeNum=instalBinary(line,0,0,0,R);
                            addNode(&head, createNodeItem(*IC, codeNum));
                            (*IC)++;
                        }
                    }
                    break;

                case rts:case stop:
                    if(line[strlen(line)-1]=='\n'){
                        line[strlen(line)-1]='\0';
                    }

                    if(thereIsSpace(line)==True){
                        fprintf(stderr,"Error in file %s: The '%s' is not valid parameter can not be with spaces in line %d \n",sourceFile,line,lineNumber);
                        errorCounter++;
                        break;
                    }

                    if(line[0]){
                        fprintf(stderr,"Error in file %s: Extraneous text after end of command '%s' in line %d \n",sourceFile,line,lineNumber);
                        errorCounter++;
                        break;
                    }

                    codeNum=instalBinary(function[command],command,0,0,A);
                    addNode(&head, createNodeItem(*IC, codeNum));
                    (*IC)++;
                    break;

                default:
                        break;
            }
        } else {
            firsStr=labelName(line);
            errorCounter++;
            fprintf(stderr,"Error in file %s: Undefined command name '%s' in line %d \n",sourceFile,firsStr,lineNumber);
            free(firsStr);
            firsStr=NULL;
            linestr=linestr->next;
            continue;
        }
        linestr=linestr->next;
    }


    freeListNodeLine(temp);
    *nodeHead=head;
    symbolTabl=table;
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

Operand *instalBinary(char *name,unsigned int opcode,unsigned int source,unsigned int target,unsigned int ARE){
    int i;
    Operand *tmp=NULL;
    tmp= malloc(sizeof (Operand));
    if(tmp==NULL){
        printf("Error: Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    tmp->lableName= strDup(name);

    if((checkData(name) == True || checkString(name) == True) || (checksExtern(name)==True || checksEntry(name)==True)){
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
             tmp->operandStucter.first->targetOperand=target; /* need to get from the function */
             tmp->operandStucter.first->opcode=i;
             tmp->operandStucter.first->sourceOperand=source; /* need to get from the function */

         }
         if(i>=not && i<=jsr){
             tmp->operandStucter.first->ARE=ARE;
             tmp->operandStucter.first->targetOperand=target;
             tmp->operandStucter.first->opcode=i;
             tmp->operandStucter.first->sourceOperand=source; /* need to get from the function */

         }
         if(i==rts || i==stop){
             tmp->operandStucter.first->ARE=ARE;
             tmp->operandStucter.first->targetOperand=0;
             tmp->operandStucter.first->opcode=i;
             tmp->operandStucter.first->sourceOperand=0;
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

void freeBinery(Operand *temp){

     if((checkData(temp->lableName) == True || checkString(temp->lableName) == True) || (checksExtern(temp->lableName)==True || checksEntry(temp->lableName)==True)) {
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

int checkLabel(char *str){
    int i;

    if(!isalpha(str[0])){
        return False;
    }

    for(i=1;i<MAX_LABLE;i++){
        if(!isalnum(str[i])){
            return False;
        }
        if(str[i]!=':' && str[i+1]=='\0'){
            return False;
        }
    }

    return True;
}

char *labelName(char *str){
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
        if(str[i]==' ' || str[i]=='\n'){
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
