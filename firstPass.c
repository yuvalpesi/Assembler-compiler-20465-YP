#include "data.h"

int firstPass(char *argv,LineHolder **nodeHead,symbolTable *symbolTabl,int *IC,int*DC){
    FILE *fd=NULL;
    char *line=NULL,*sourceFile=NULL,*tempToken=NULL,*secStr=NULL,*firsStr=NULL,*labelname=NULL;
    int command,number,c,address=100,strSpace,index=0,errorCounter=0,lineNumber=0;
    symbolTable *table=symbolTabl;
    LineHolder *head=*nodeHead;
    Operand *codeNum=NULL;
    lineStr *linestr=NULL,*temp=NULL;
    char *function[FUNCTON_NUM]={ "mov","cmp","add","sub",
                                  "not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};

    sourceFile=(char*)malloc((strlen(argv)+4) * sizeof(char));
    line=(char*)calloc(MAX_LINE_LENGHT + 2,sizeof(char));

    if(sourceFile == NULL || line==NULL){
        printf(" Failed to allocate memory.\n");
        exit(0);
    }

    strncpy(sourceFile,argv,strlen(argv));
    sourceFile[strlen(argv)]='\0';
    strcat(sourceFile,".am");
    sourceFile[strlen(sourceFile)]='\0';


    if((fd=fopen(sourceFile,"r"))==NULL){
        printf(" Error cant open file\n");
        exit(0);
    }

    while (!feof(fd) && fgets(line,MAX_LINE_LENGHT+2,fd)!=NULL){
        addNodeLine(&linestr, createNodeLine(line));
    }

    free(line);
    line=NULL;
    temp=linestr;

    while(linestr!=NULL){
        lineNumber++;
        if(line!=NULL){
            free(line);
            line=NULL;
        }
        line=strDup(linestr->lineStr);
        TOO_LONG_LINE(line,lineNumber,errorCounter)
        labelname=labelName(line);

        if(labelname[strlen(labelname)-1]==':' && (thereIsSpace(labelname)==True || !isalpha(labelname[0]))){
            fprintf(stderr,"The %s is not valid Label in line %d \n",labelname,lineNumber);
            errorCounter++;
            free(labelname);
            linestr=linestr->next;
            continue;
        }

        if(labelname[strlen(labelname)-1]==':'  && checksEntry(line+ strlen(labelname))!=True){
            if(strlen(labelname)>MAX_LABLE){
                fprintf(stderr,"The %s is not valid Label length too big in line %d \n",labelname,lineNumber);
                errorCounter++;
                free(labelname);
                linestr=linestr->next;
                continue;
            }

            strSpace = strlen(labelname); /* new */
            memmove(line,line+strSpace, strlen(line)+1);
            /*line+=strSpace;*/
            line=ignorSpace(line);

            tempToken=(char *) malloc(strSpace*sizeof (char));
            if(tempToken==NULL){
                printf(" Failed to allocate memory.\n");
                exit(0);
            }

            strcpy(tempToken,labelname);

            if(checkLabel(tempToken)==False && strlen(tempToken)>MAX_LABLE){
                fprintf(stderr,"The %s Label too long can not pass 31 chars in line %d \n",tempToken,lineNumber);
                errorCounter++;
                free(tempToken);
                linestr=linestr->next;
                continue;
            }

            tempToken[strlen(tempToken)-1]='\0'; /* remover the ':' */

            if(checkRegister(tempToken)!=ERROR || commandType(tempToken)!=ERROR){
                fprintf(stderr,"The %s is not valid Label (assembly Reserved Words) in line %d \n",tempToken,lineNumber);
                errorCounter++;
                free(tempToken);
                linestr=linestr->next;
                continue;
            }

            if(symbolTableLockUp(table,tempToken) && symbolTableLockUpType(table,tempToken)==ERROR){
                fprintf(stderr,"The %s is not valid Label (already use) in line %d \n",tempToken,lineNumber);
                errorCounter++;
                free(tempToken);
                linestr=linestr->next;
                continue;
            }

            symbolTableInsert(table,tempToken,address,NoEntryOrExtern);
            free(tempToken);
        }

        if(labelname[strlen(labelname)-1]==':'  && checksEntry(line+ strlen(labelname))==True){
            strSpace = strlen(labelname); /* new */
            memmove(line,line+strSpace, strlen(line)+1);
            /*line+=strSpace;*/
            line=ignorSpace(line);
            free(labelname);
        }

        if(labelname!=NULL){
            free(labelname);
        }

        if((checkData(line) == True || checkString(line) == True )) {
            if (checkString(line) == True ) {
                secStr= strDup(line);
                secStr[8]='\0';
                memmove(line,line+7, strlen(line)+1);
                /*line+=7;*/
                /*RemoveAllSpaces(line);*/
                line= ignorSpace(line);
                if(line[strlen(line)-1]=='\n'){
                    line[strlen(line)-1]='\0';
                }
                if(line[0]!='"' || line[strlen(line)-1]!='"'){
                    fprintf(stderr,"ERROR: %s is not valid string (missing quotation marks) in line %d \n",line,lineNumber);
                    errorCounter++;
                    linestr=linestr->next;
                    continue;
                }
                RemoveQuotationMarks(line);
                index=0;
                while (line[index] != '\0'){
                    c=(unsigned char)line[index];
                    codeNum= instalBinary(codeNum,secStr,c,0,0,0);
                    addNode(&head, createNodeItem(address++, codeNum));
                    index++;
                    (*DC)++;
                    if (line[index] == '\0'){
                        codeNum= instalBinary(codeNum,secStr,'\0',0,0,0);
                        addNode(&head, createNodeItem(address++, codeNum));
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
                /*line+=5;*/
                RemoveAllSpaces(line);

                index = 0;
                ILLEGAL_COMMA(line[index],lineNumber,errorCounter)
                while (line[index] != '\0' && index<strlen(line)) {

                    if(!isdigit(line[index]) && line[index]!=',' && line[index]!='\0' && line[index]!='+'){
                        MISSING_PARAMITER_DATA(line[index],lineNumber,errorCounter,index)
                        INVALID_PARAMETER_NOT_A_NUMBER(line[index], line[index+1], lineNumber, errorCounter, index)
                    }

                    if(line[index+1]=='\0'&& !isdigit(line[index])){
                        EXTRANEOUS_TEXT_AFTER_END_OF_COMMAND(line[strlen(line)-1],lineNumber,errorCounter,index)
                    }

                    if(line[index]==',' && line[index+1]==','){
                        MULIPLE_CONSECUTIVE_COMMAS_DATA(line[index],lineNumber,errorCounter,index)

                    }

                    if(line[index] != ',' && line[index] != '\0'){
                        memmove(line,line+index, strlen(line)+1);
                        number=getNumberFromData(line,&index);

                        if(number>MAX_NUMBER_SIZE || number<LOW_NUMBER_SIZE){
                            if(number<0){
                                fprintf(stderr,"Invalid parameter in line %d this number: %d is too small\n",lineNumber,number);
                            }else{
                                fprintf(stderr,"Invalid parameter in line %d this number: %d is too big\n",lineNumber,number);
                            }
                            index++;
                            errorCounter++;
                            continue;
                        }

                        codeNum= instalBinary(codeNum,secStr,number,0,0,0);
                        addNode(&head, createNodeItem(address++, codeNum));
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
                /*secStr+=8;*/
                RemoveAllSpaces(secStr);
                symbolTableInsert(table,secStr,0,sEXETRN);
                linestr=linestr->next;
                free(secStr);
                continue;
            } else if(checksEntry(line)==True){
                secStr=strDup(line);
                memmove(secStr,secStr+7, strlen(line)+1);
                /*secStr+=7;*/
                RemoveAllSpaces(secStr);
                symbolTableInsert(table,secStr,0,sENTRY);
                linestr=linestr->next;
                free(secStr);
                continue;
            }
        }

        line=ignorSpace(line);
        command = commandType(line);


        if (command != ERROR) {
            memmove(line,line+strlen(function[command]), strlen(line)+1);
            /*line+=strlen(function[command]);*/
            RemoveAllSpaces(line);
            ILLEGAL_COMMA(line[0],lineNumber,errorCounter)
            switch (command) {
                case mov:case cmp:case add:case lea:case sub:
                    firsStr=strFirstArgu(line);
                    MISSING_PARAMITER(firsStr[0],lineNumber,errorCounter)
                    memmove(line,line+(strlen(firsStr)), strlen(line)+1);
                    /*line+= (strlen(firsStr));  sec argu */
                    MISSING_COMMA(line[0],lineNumber,errorCounter)
                    memmove(line,line+1, strlen(line)+1);
                    /*line+=1;*/
                    MISSING_PARAMITER(line[0],lineNumber,errorCounter)
                    MULIPLE_CONSECUTIVE_COMMAS(line[0],lineNumber,errorCounter)

                    if(firsStr[0]=='@'){
                        UNDEFINED_REGISTER_NUMBER(checkRegister(firsStr),lineNumber,errorCounter)
                    }

                    if(line[0]=='@'){
                        UNDEFINED_REGISTER_NUMBER(checkRegister(line),lineNumber,errorCounter)
                    }

                    if(command!=lea && checkRegister(firsStr)!= ERROR && checkRegister(line)!=ERROR){

                        if(checkRegister(firsStr)!=ERROR && checkRegister(line)!=ERROR){
                            codeNum=instalBinary(codeNum,function[command],command,registerDirectAddressing,registerDirectAddressing,A);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                        }

                        if(checkRegister(firsStr)!=ERROR && checkRegister(line)!=ERROR){
                            codeNum=instalBinary(codeNum,firsStr,0,checkRegister(firsStr),checkRegister(line),A);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                        }

                    } else if(command==lea){ /* lea target   */

                        if(allDigits(firsStr)==True || checkRegister(firsStr)!=ERROR){
                            errorCounter++;
                            fprintf(stderr,"Undefined operand type name %s in line %d \n",firsStr,lineNumber);
                            break;
                        }

                        if(checkRegister(line)!=ERROR){
                            if(allDigits(firsStr)!=True){
                               codeNum=instalBinary(codeNum,function[command],command,directAddressing,registerDirectAddressing,A);
                               addNode(&head, createNodeItem(address++, codeNum));
                               (*IC)++;
                            }

                        } else if(allDigits(firsStr)!=True && allDigits(line)!=True){
                            if(checkLabelInCommand(firsStr)!=True){
                                fprintf(stderr,"The %s is not valid Label in line %d \n",firsStr,lineNumber);
                                errorCounter++;
                                break;
                            }
                            if(checkLabelInCommand(line)!=True){
                                fprintf(stderr,"The %s is not valid Label in line %d \n",line,lineNumber);
                                errorCounter++;
                                break;
                            }
                            codeNum=instalBinary(codeNum,function[command],command,directAddressing, directAddressing,A);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                        }


                        /* first argu */
                        codeNum=instalBinary(codeNum,firsStr,0,0,0,R);
                        addNode(&head, createNodeItem(address++, codeNum));
                        (*IC)++;

                        /* sec argu */
                        if(checkRegister(line)!=ERROR){
                            number=checkRegister(line);
                            codeNum=instalBinary(codeNum,line,0,0,number,A);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                        } else if(allDigits(line)!=True){
                            codeNum=instalBinary(codeNum,line,0,0,0,R);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                        }

                    } else { /* all the rest*/

                        if(command!=cmp && allDigits(line)==True){
                            errorCounter++;
                            fprintf(stderr,"Undefined operand type name %s in line %d \n",line,lineNumber);
                            break;
                        }

                        if(checkRegister(firsStr)!=ERROR || checkRegister(line)!=ERROR){
                            if(checkRegister(firsStr)!=ERROR){
                                if(allDigits(line)!=True){
                                    codeNum=instalBinary(codeNum,function[command],command,registerDirectAddressing,directAddressing,A);
                                    addNode(&head, createNodeItem(address++, codeNum));
                                    (*IC)++;
                                }

                                if(allDigits(line)==True && command==cmp){
                                    codeNum=instalBinary(codeNum,function[command],command,registerDirectAddressing,immediateAddressing,A);
                                    addNode(&head, createNodeItem(address++, codeNum));
                                    (*IC)++;
                                }

                            }else if(checkRegister(line)!=ERROR){
                                if(allDigits(firsStr)==True){
                                    codeNum=instalBinary(codeNum,function[command],command,immediateAddressing,registerDirectAddressing,A);
                                    addNode(&head, createNodeItem(address++, codeNum));
                                    (*IC)++;
                                }

                                if(allDigits(firsStr)!=True){
                                    codeNum=instalBinary(codeNum,function[command],command,directAddressing,registerDirectAddressing,A);
                                    addNode(&head, createNodeItem(address++, codeNum));
                                    (*IC)++;
                                }
                            }

                        }else if(allDigits(firsStr)==True && checkRegister(firsStr)==ERROR && checkRegister(line)==ERROR){
                            if(allDigits(line)!=True){
                                codeNum=instalBinary(codeNum,function[command],command,immediateAddressing,directAddressing,A);
                                addNode(&head, createNodeItem(address++, codeNum));
                                (*IC)++;
                            }

                            if(allDigits(line)==True && command==cmp){
                                codeNum=instalBinary(codeNum,function[command],command,immediateAddressing,immediateAddressing,A);
                                addNode(&head, createNodeItem(address++, codeNum));
                                (*IC)++;
                            }

                        }else if(allDigits(firsStr)!=True && checkRegister(firsStr)==ERROR && checkRegister(line)==ERROR){
                            if(allDigits(line)!=True) {
                                codeNum = instalBinary(codeNum, function[command], command, directAddressing,directAddressing,A);
                                addNode(&head, createNodeItem(address++, codeNum));
                                (*IC)++;
                            }

                            if(allDigits(line)==True && command==cmp){
                                codeNum=instalBinary(codeNum,function[command],command,directAddressing,immediateAddressing,A);
                                addNode(&head, createNodeItem(address++, codeNum));
                                (*IC)++;
                            }
                        }

                        /* first argu */
                        if(allDigits(firsStr)==True){
                            number= atoi(firsStr);

                            if(number>WOLD_NUMBER_MAX_SIZE || number<WOLD_NUMBER_LOW_SIZE){
                                if(number<0){
                                    fprintf(stderr,"Invalid parameter in line %d this number: %d is too small\n",lineNumber,number);
                                }else{
                                    fprintf(stderr,"Invalid parameter in line %d this number: %d is too big\n",lineNumber,number);
                                }
                                index++;
                                errorCounter++;
                                break;
                            }

                            codeNum=instalBinary(codeNum,firsStr,number,0,0,A);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                        } else if(checkRegister(firsStr)!=ERROR){
                            number= checkRegister(firsStr);
                            codeNum=instalBinary(codeNum,firsStr,0,number,0,A);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                        } else {
                            if(checkLabelInCommand(firsStr)!=True){
                                fprintf(stderr,"The %s is not valid Label in line %d \n",firsStr,lineNumber);
                                errorCounter++;
                                break;
                            }

                            codeNum=instalBinary(codeNum,firsStr,0,0,0,R);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                        }

                        /* sec argu */
                         if(checkRegister(line)!=ERROR){
                             number=checkRegister(line);
                             codeNum=instalBinary(codeNum,line,0,0,number,A);
                             addNode(&head, createNodeItem(address++, codeNum));
                             (*IC)++;
                        } else if(allDigits(line)!=True){
                             if(checkLabelInCommand(line)!=True){
                                 fprintf(stderr,"The %s is not valid Label in line %d \n",line,lineNumber);
                                 errorCounter++;
                                 break;
                             }

                            codeNum=instalBinary(codeNum,line,0,0,0,R);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                        } else if(command==cmp && allDigits(line)==True){
                             number= atoi(firsStr);

                             if(number>WOLD_NUMBER_MAX_SIZE || number<WOLD_NUMBER_LOW_SIZE){
                                 if(number<0){
                                     fprintf(stderr,"Invalid parameter in line %d this number: %d is too small\n",lineNumber,number);
                                 }else{
                                     fprintf(stderr,"Invalid parameter in line %d this number: %d is too big\n",lineNumber,number);
                                 }
                                 index++;
                                 errorCounter++;
                                 break;
                             }

                             codeNum=instalBinary(codeNum,line,number,0,0,A);
                             addNode(&head, createNodeItem(address++, codeNum));
                             (*IC)++;
                         }
                    }
                    free(firsStr);
                    firsStr=NULL;
                    break;

                case not:case clr:case inc:case dec:case jmp:case bne:case red:case prn:case jsr:

                    MISSING_PARAMITER(line[0],lineNumber,errorCounter)
                    /*EXTRANEOUS_TEXT_AFTER_END_OF_COMMAND(line[3],lineNumber,errorCounter,index)*/

                    if(line[0]=='@'){
                        UNDEFINED_REGISTER_NUMBER(checkRegister(line),lineNumber,errorCounter)
                    }

                    if(command!=prn && allDigits(line)==True){
                        errorCounter++;
                        fprintf(stderr,"Undefined operand type name %s in line %d \n",line,lineNumber);
                        break;
                    }

                    if(command==prn){

                        if(allDigits(line)==True){
                            codeNum=instalBinary(codeNum,function[command],command,0,immediateAddressing,A);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                            number=atoi(line);

                            if(number>WOLD_NUMBER_MAX_SIZE || number<WOLD_NUMBER_LOW_SIZE){
                                if(number<0){
                                    fprintf(stderr,"Invalid parameter in line %d this number: %d is too small\n",lineNumber,number);
                                }else{
                                    fprintf(stderr,"Invalid parameter in line %d this number: %d is too big\n",lineNumber,number);
                                }
                                errorCounter++;
                                break;
                            }

                            codeNum=instalBinary(codeNum,line,number,0,0,A);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;

                        } else if(checkRegister(line)!=ERROR){
                            codeNum=instalBinary(codeNum,function[command],command,0,registerDirectAddressing,A);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                            codeNum=instalBinary(codeNum,line,0,0,checkRegister(line),A);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;

                        } else{

                            if(checkLabelInCommand(line)!=True){
                                fprintf(stderr,"The %s is not valid Label in line %d \n",line,lineNumber);
                                errorCounter++;
                                break;
                            }

                            codeNum=instalBinary(codeNum,function[command],command,0,directAddressing,A);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;

                            codeNum=instalBinary(codeNum,line,0,0,0,R);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                        }

                    }else{

                         if(checkRegister(line)!=ERROR){
                            codeNum=instalBinary(codeNum,function[command],command,0,registerDirectAddressing,A);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                            codeNum=instalBinary(codeNum,line,0,0,checkRegister(line),A);
                             addNode(&head, createNodeItem(address++, codeNum));
                             (*IC)++;

                        } else if(allDigits(line)!=True){

                             if(checkLabelInCommand(line)!=True){
                                 fprintf(stderr,"The %s is not valid Label in line %d \n",line,lineNumber);
                                 errorCounter++;
                                 break;
                             }

                            codeNum=instalBinary(codeNum,function[command],command,0,directAddressing,A);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                            codeNum=instalBinary(codeNum,line,0,0,0,R);
                            addNode(&head, createNodeItem(address++, codeNum));
                            (*IC)++;
                        }
                    }
                    break;

                case rts:case stop:
                    EXTRANEOUS_TEXT_AFTER_END_OF_COMMAND(line[0],lineNumber,errorCounter,c)
                    codeNum=instalBinary(codeNum,function[command],command,0,0,A);
                    addNode(&head, createNodeItem(address++, codeNum));
                    (*IC)++;
                    break;

                default:
                        break;
            }
        } else {
            firsStr=labelName(line);
            errorCounter++;
            fprintf(stderr,"Undefined command name %s in line %d \n",firsStr,lineNumber);
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

Operand *instalBinary(Operand *temp,char *name,unsigned int opcode,unsigned int source,unsigned int target,unsigned int ARE){
    int i;
    Operand *tmp=NULL;
    tmp= malloc(sizeof (Operand));
    if(tmp==NULL){
        printf(" Failed to allocate memory.\n");
        exit(0);
    }
    tmp->lableName= strDup(name);

    if((checkData(name) == True || checkString(name) == True) || (checksExtern(name)==True || checksEntry(name)==True)){
        tmp->operandStucter.directiveSentence= malloc(sizeof (DirectiveSentence));

        if(tmp->operandStucter.directiveSentence==NULL){
            printf(" Failed to allocate memory.\n");
            exit(0);
        }

        tmp->operandStucter.directiveSentence->directive=opcode;

    }else if((i= commandType(name))!=ERROR){
        tmp->operandStucter.first= malloc(sizeof (firstOp));

        if(tmp->operandStucter.first==NULL){
            printf(" Failed to allocate memory.\n");
            exit(0);
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
            printf(" Failed to allocate memory.\n");
            exit(0);
        }

        tmp->operandStucter.immediate->ARE=ARE;
        tmp->operandStucter.immediate->Operand=opcode;
    } else if(checkRegister(name)!=ERROR){
         tmp->operandStucter.registerAddress= malloc(sizeof (registerDirectAddressing));

         if(tmp->operandStucter.registerAddress==NULL){
             printf(" Failed to allocate memory.\n");
             exit(0);
         }

         tmp->operandStucter.registerAddress->ARE=ARE;
         tmp->operandStucter.registerAddress->FirstRed=target;
        tmp->operandStucter.registerAddress->SecRed=source;

    } else if(allDigits(name)!=True){
        tmp->operandStucter.address= malloc(sizeof (DirectAddress));

        if(tmp->operandStucter.address==NULL){
            printf(" Failed to allocate memory.\n");
            exit(0);
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
                printf(" Failed to allocate memory.\n");
                exit(0);
            }

            strncpy(temp,str,i+1);
            temp[i+1]='\0';
            return temp;
        }
        if(str[i]==' ' || str[i]=='\n'){
            temp=(char *) malloc(sizeof (char ) *(i+2));

            if(temp==NULL){
                printf(" Failed to allocate memory.\n");
                exit(0);
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
    int i; /*8**/

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