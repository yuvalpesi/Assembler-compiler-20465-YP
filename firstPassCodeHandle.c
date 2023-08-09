#include "data.h"

void handleNoOperand(int command,char *function, char *line, char *sourceFile, int lineNumber, int *errorCounter, int* IC, LineHolder **head){
    Operand *codeNum=NULL;

    if(thereIsSpace(line)==True){
        fprintf(stderr,"Error in file %s: The '%s' is not valid parameter can not be with spaces in line %d \n",sourceFile,line,lineNumber);
        (*errorCounter)++;
        return;
    }

    if(line[0]){
        fprintf(stderr,"Error in file %s: Extraneous text after end of command '%s' in line %d \n",sourceFile,line,lineNumber);
        (*errorCounter)++;
        return;
    }
    /* crate the binary operand */
    codeNum=installBinary(function,command,0,0,A);
    /* crate and add the new item addressing image linked list node */
    addNode(head, createNodeItem(*IC, codeNum));
    (*IC)++;/* add one to the code image */
}

void handleOneOperand(int command,char *function, char *Line, char *sourceFile, int lineNumber, int *errorCounter, int* IC, LineHolder **head){
    Operand *codeNum=NULL;
    int number;
    char *firsStr= strFirstArgu(Line),*line= strDup(Line);/* set a copy of Line */

    memmove(line,line+ strlen(firsStr), strlen(line));

    if(line[0]=='\0'){
        free(line);
        line=NULL;
    }
    line= ignoreSpace(line);

    if( line!=NULL && (line[0]==',' || thereIsSpace(line)|| findComma(line)==True || allDigits(line) ||
                       allDigits(line)!=True)){
        fprintf(stderr,"Error in file %s: Extraneous text after end of command '%s' in line number %d \n",sourceFile,line,lineNumber);
        (*errorCounter)++;
        free(line);
        free(firsStr);
        return;
    }

    line= strDup(firsStr);
    free(firsStr);
    firsStr=NULL;

    if(!line[0]){
        fprintf(stderr,"Error in file %s: Missing parameter in line %d \n",sourceFile,lineNumber);
        (*errorCounter)++;
        free(line);
        return;
    }

    if(line[0]=='@'){
        if(checkRegister(line)==ERROR){
            (*errorCounter)++;
            fprintf(stderr,"Error in file %s: Undefined register number name '%s' in line %d \n",sourceFile,line,lineNumber);
            free(line);
            return;
        }
    }

    if(command!=prn && allDigits(line)==True){
        (*errorCounter)++;
        fprintf(stderr,"Error in file %s: Undefined operand type name '%s' in line %d \n",sourceFile,line,lineNumber);
        free(line);
        return;
    }

    if(command==prn){

        if(allDigits(line)==True){
            codeNum=installBinary(function,command,0,immediateAddressing,A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;

            if(checkRealNumber(line)==True){
                fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %s is real number and the assembler dose not support real numbers\n",sourceFile,lineNumber,line);
                (*errorCounter)++;
                free(line);
                return;
            }
            number=atoi(line); /* get the number from the parameter */

            if(number>WOLD_NUMBER_MAX_SIZE || number<WOLD_NUMBER_LOW_SIZE){
                if(number<0){
                    fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too small\n",sourceFile,lineNumber,number);
                }else{
                    fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too big\n",sourceFile,lineNumber,number);
                }
                (*errorCounter)++;
                free(line);
                return;
            }

            codeNum=installBinary(line,number,0,0,A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;

        } else if(checkRegister(line)!=ERROR){
            codeNum=installBinary(function,command,0,registerDirectAddressing,A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
            codeNum=installBinary(line,0,0,checkRegister(line),A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;

        } else{/* if the parameter is a label */

            if(checkLabelInCommand(line)!=True){
                fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,line,lineNumber);
                (*errorCounter)++;
                free(line);
                return;
            }

            codeNum=installBinary(function,command,0,directAddressing,A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;

            codeNum=installBinary(line,0,0,0,R);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
        }

    }else{ /* if the command is not a prn */

        if(checkRegister(line)!=ERROR){
            codeNum=installBinary(function,command,0,registerDirectAddressing,A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
            codeNum=installBinary(line,0,0,checkRegister(line),A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;

        } else if(allDigits(line)!=True){

            if(checkLabelInCommand(line)!=True){
                fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,line,lineNumber);
                (*errorCounter)++;
                free(line);
                return;
            }

            codeNum=installBinary(function,command,0,directAddressing,A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
            codeNum=installBinary(line,0,0,0,R);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
        }
    }

    if(firsStr!=NULL){
        free(firsStr);
    }
    free(line);
}

void handleTowOperand(int command,char *function, char *Line, char *sourceFile, int lineNumber, int *errorCounter, int* IC, LineHolder **head){
    Operand *codeNum=NULL;
    int number;
    char *firsStr= strFirstArgu(Line),*line= strDup(Line);/* set a copy of Line */

    /* the first parameter in the line */
    if(!firsStr[0]){
        fprintf(stderr,"Error in file %s: Missing parameter in line %d \n",sourceFile,lineNumber);
        (*errorCounter)++;
        free(line);
        free(firsStr);
        return;
    }

    memmove(line,line+(strlen(firsStr)), strlen(line)+1); /* remove the first parameter and stay with the sec parameter in the line */
    if(firsStr[strlen(firsStr)-1]==' '){
        firsStr[strlen(firsStr)-1]='\0';
    }
    line= ignoreSpace(line);

    if(line[0]!=','){
        fprintf(stderr,"Error in file %s: Missing comma '%s' in line %d \n",sourceFile,line,lineNumber);
        (*errorCounter)++;
        free(line);
        free(firsStr);
        return;
    }

    memmove(line,line+1, strlen(line)+1); /* remove the comma from the sec parameter*/

    if(!line[0]){
        fprintf(stderr,"Error in file %s: Missing parameter in line %d \n",sourceFile,lineNumber);
        (*errorCounter)++;
        free(line);
        free(firsStr);
        return;
    }

    if(line[0]==','){
        fprintf(stderr,"Error in file %s: Multiple consecutive commas '%s' in line %d \n",sourceFile,line,lineNumber);
        (*errorCounter)++;
        free(line);
        free(firsStr);
        return;
    }

    line= ignoreSpace(line);

    if(thereIsSpace(line)==True || thereIsSpace(firsStr)==True){
        if(thereIsSpace(firsStr)==True){
            fprintf(stderr,"Error in file %s: The '%s' is not valid parameter can not be with spaces in line %d \n",sourceFile,firsStr,lineNumber);
            (*errorCounter)++;
            free(line);
            free(firsStr);
            return;
        } else{
            fprintf(stderr,"Error in file %s: Extraneous text after end of command '%s' in line %d \n",sourceFile,line,lineNumber);
            (*errorCounter)++;
            free(line);
            free(firsStr);
            return;
        }
    }

    if(line[strlen(line)-1]==','){
        fprintf(stderr,"Error in file %s: Extraneous text after end of command '%s' in line %d \n",sourceFile,line,lineNumber);
        (*errorCounter)++;
        free(line);
        free(firsStr);
        return;
    }

    if(firsStr[0]=='@'){
        if(checkRegister(firsStr)==ERROR){
            (*errorCounter)++;
            fprintf(stderr,"Error in file %s: Undefined register number name '%s' in line %d \n",sourceFile,firsStr,lineNumber);
            free(line);
            free(firsStr);
            return;
        }
    }

    if(line[0]=='@'){
        if(checkRegister(line)==ERROR){
            (*errorCounter)++;
            fprintf(stderr,"Error in file %s: Undefined register number name '%s' in line %d \n",sourceFile,line,lineNumber);
            free(line);
            free(firsStr);
            return;
        }
    }
    /* see if in the operands there is 2 registers and the comma is not lea*/
    if(command!=lea && checkRegister(firsStr)!= ERROR && checkRegister(line)!=ERROR){

        if(checkRegister(firsStr)!=ERROR && checkRegister(line)!=ERROR){
            /* crate the opcode for the command arguments */
            codeNum=installBinary(function,command,registerDirectAddressing,registerDirectAddressing,A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
        }

        if(checkRegister(firsStr)!=ERROR && checkRegister(line)!=ERROR){
            /* crate the opcode for the operand arguments */
            codeNum=installBinary(firsStr,0,checkRegister(firsStr),checkRegister(line),A);
            addNode(head, createNodeItem(*IC, codeNum)); /* add to the node address list  */
            (*IC)++;
        }

    } else if(command==lea){ /* lea command target  analyze */

        if(allDigits(firsStr)==True || checkRegister(firsStr)!=ERROR){
            (*errorCounter)++;
            fprintf(stderr,"Error in file %s: Undefined operand type name '%s' in line %d \n",sourceFile,firsStr,lineNumber);
            free(line);
            free(firsStr);
            return;
        }

        if(allDigits(line)==True){
            (*errorCounter)++;
            fprintf(stderr,"Error in file %s: Undefined operand type name '%s' in line %d \n",sourceFile,line,lineNumber);
            free(line);
            free(firsStr);
            return;
        }

        if(checkRegister(line)!=ERROR){
            if(allDigits(firsStr)!=True){
                codeNum=installBinary(function,command,directAddressing,registerDirectAddressing,A);
                addNode(head, createNodeItem(*IC, codeNum));
                (*IC)++;
            }

        } else if(allDigits(firsStr)!=True && allDigits(line)!=True){
            if(checkLabelInCommand(firsStr)!=True){
                fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,firsStr,lineNumber);
                (*errorCounter)++;
                free(line);
                free(firsStr);
                return;
            }
            if(checkLabelInCommand(line)!=True){
                fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,line,lineNumber);
                (*errorCounter)++;
                free(line);
                free(firsStr);
                return;
            }
            codeNum=installBinary(function,command,directAddressing, directAddressing,A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
        }

        /* first operand opcode argument */
        codeNum=installBinary(firsStr,0,0,0,R);
        addNode(head, createNodeItem(*IC, codeNum));
        (*IC)++;

        /* sec operand opcode argument */
        if(checkRegister(line)!=ERROR){
            number=checkRegister(line);
            codeNum=installBinary(line,0,0,number,A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
        } else if(allDigits(line)!=True){
            codeNum=installBinary(line,0,0,0,R);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
        }

    } else { /* all the rest commands in that case which is not holding 2 registers and not lea command */

        if(command!=cmp && allDigits(line)==True){
            (*errorCounter)++;
            fprintf(stderr,"Error in file %s: Undefined operand type name '%s' in line %d \n",sourceFile,line,lineNumber);
            free(line);
            free(firsStr);
            return;
        }

        if(checkRegister(firsStr)!=ERROR || checkRegister(line)!=ERROR){
            if(checkRegister(firsStr)!=ERROR){
                if(allDigits(line)!=True){
                    codeNum=installBinary(function,command,registerDirectAddressing,directAddressing,A);
                    addNode(head, createNodeItem(*IC, codeNum));
                    (*IC)++;
                }

                if(allDigits(line)==True && command==cmp){
                    codeNum=installBinary(function,command,registerDirectAddressing,immediateAddressing,A);
                    addNode(head, createNodeItem(*IC, codeNum));
                    (*IC)++;
                }

            }else if(checkRegister(line)!=ERROR){
                if(allDigits(firsStr)==True){
                    codeNum=installBinary(function,command,immediateAddressing,registerDirectAddressing,A);
                    addNode(head, createNodeItem(*IC, codeNum));
                    (*IC)++;
                }

                if(allDigits(firsStr)!=True){
                    codeNum=installBinary(function,command,directAddressing,registerDirectAddressing,A);
                    addNode(head, createNodeItem(*IC, codeNum));
                    (*IC)++;
                }
            }

        }else if(allDigits(firsStr)==True && checkRegister(firsStr)==ERROR && checkRegister(line)==ERROR){
            if(allDigits(line)!=True){
                codeNum=installBinary(function,command,immediateAddressing,directAddressing,A);
                addNode(head, createNodeItem(*IC, codeNum));
                (*IC)++;
            }

            if(allDigits(line)==True && command==cmp){
                codeNum=installBinary(function,command,immediateAddressing,immediateAddressing,A);
                addNode(head, createNodeItem(*IC, codeNum));
                (*IC)++;
            }

        }else if(allDigits(firsStr)!=True && checkRegister(firsStr)==ERROR && checkRegister(line)==ERROR){
            if(allDigits(line)!=True) {
                codeNum = installBinary( function, command, directAddressing,directAddressing,A);
                addNode(head, createNodeItem(*IC, codeNum));
                (*IC)++;
            }

            if(allDigits(line)==True && command==cmp){
                codeNum=installBinary(function,command,directAddressing,immediateAddressing,A);
                addNode(head, createNodeItem(*IC, codeNum));
                (*IC)++;
            }
        }

        /* first operand opcode build argument */
        if(allDigits(firsStr)==True){
            if(checkRealNumber(firsStr)==True){
                fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %s is real number and the assembler dose not support real numbers\n",sourceFile,lineNumber,firsStr);
                (*errorCounter)++;
                free(line);
                free(firsStr);
                return;
            }
            number= atoi(firsStr);

            if(number>WOLD_NUMBER_MAX_SIZE || number<WOLD_NUMBER_LOW_SIZE){
                if(number<0){
                    fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too small\n",sourceFile,lineNumber,number);
                }else{
                    fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too big\n",sourceFile,lineNumber,number);
                }
                (*errorCounter)++;
                free(line);
                free(firsStr);
                return;
            }

            codeNum=installBinary(firsStr,number,0,0,A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
        } else if(checkRegister(firsStr)!=ERROR){
            number= checkRegister(firsStr);
            codeNum=installBinary(firsStr,0,number,0,A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
        } else {
            if(checkLabelInCommand(firsStr)!=True){
                fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,firsStr,lineNumber);
                (*errorCounter)++;
                free(line);
                free(firsStr);
                return;
            }

            codeNum=installBinary(firsStr,0,0,0,R);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
        }

        /* sec operand opcode build argument  */
        if(checkRegister(line)!=ERROR){
            number=checkRegister(line);
            codeNum=installBinary(line,0,0,number,A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
        } else if(allDigits(line)!=True){
            if(checkLabelInCommand(line)!=True){
                fprintf(stderr,"Error in file %s: The '%s' is not valid Label in line %d \n",sourceFile,line,lineNumber);
                (*errorCounter)++;
                free(line);
                free(firsStr);
                return;
            }

            codeNum=installBinary(line,0,0,0,R);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
        } else if(command==cmp && allDigits(line)==True){
            if(checkRealNumber(line)==True){
                fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %s is real number and the assembler dose not support real numbers\n",sourceFile,lineNumber,line);
                (*errorCounter)++;
                free(line);
                free(firsStr);
                return;
            }
            number= atoi(line);

            if(number>WOLD_NUMBER_MAX_SIZE || number<WOLD_NUMBER_LOW_SIZE){
                if(number<0){
                    fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too small\n",sourceFile,lineNumber,number);
                }else{
                    fprintf(stderr,"Error in file %s: Invalid parameter in line %d, this number: %d is too big\n",sourceFile,lineNumber,number);
                }
                (*errorCounter)++;
                free(line);
                free(firsStr);
                return;
            }

            codeNum=installBinary(line,number,0,0,A);
            addNode(head, createNodeItem(*IC, codeNum));
            (*IC)++;
        }
    }

    free(firsStr);
    free(line);
}
