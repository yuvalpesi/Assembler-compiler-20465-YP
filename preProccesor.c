#include "data.h"

int preProccesor(char *inputFileName){
    FILE *inputFile=NULL,*expandFile=NULL;
    char **splitLine=NULL,*mcroName=NULL,*updatedMacro=NULL,*line=NULL,*sourceFile=NULL;
    int i=0,wordsCounter=0,fileSize=strlen(inputFileName)+4;
    boolean readMacroFlag= False;
    mcro *macroTable=initMcroTable();

    sourceFile=(char*)malloc(fileSize*sizeof(char));
    line=(char*)calloc(MAX_LINE_LENGHT + 2,sizeof(char));
    if(sourceFile == NULL || line==NULL){
        printf(" Failed to allocate memory.\n");
        return False;
    }

    strncpy(sourceFile,inputFileName,strlen(inputFileName)); /* copy to the sourceFile the inputfilename */
    strcat(sourceFile,".as");

    /* If the file can not be open it will tell to the user*/
    if(!(inputFile=fopen(sourceFile, "r"))){
        fprintf(stderr,"\n Error: Cannot open the file %s!\n",inputFileName);
        return False;
    }

    sourceFile[strlen(sourceFile)-1]='m'; /* counvert the files to .am file */

    /* If the expanded soure file can not creat it will tell to the user */
    if(!(expandFile = fopen(sourceFile, "w+")) ){
        fprintf(stderr, "\n Error: Failed to create the expanded source code file %s. \n", sourceFile);
        fclose(inputFile);
        return False;
    }

    while(!feof(inputFile) && fgets(line,MAX_LINE_LENGHT+2,inputFile)!=NULL){
        wordsCounter=0;
        line=ignorSpace(line);

        if(line[i]=='\n' || line[i]=='\0' || line[i]==EOF || line[i]==';'){
            continue;
        }

        splitLine=splitStringLine(line ,&wordsCounter);

        if(mcroTableLockUp(macroTable, splitLine[0])!=NULL){
            fputs(mcroTableLockUp(macroTable, splitLine[0]), expandFile);

        }else if (strcmp(splitLine[0], "mcro") == 0){
            readMacroFlag = True;
            mcroName=(char *)malloc(sizeof(char)*MCRO_NAME_LENGHT);
            strcpy(mcroName,splitLine[1]);
            if(checkRegister(mcroName)!=ERROR || checksEntry(mcroName)!=False || checksExtern(mcroName)!=False ||
                    checkData(mcroName)!=False || checkString(mcroName)!=False || commandType(mcroName)!=ERROR){
                remove(sourceFile);
                fprintf(stderr,"Error: macro name is assembler Reserved Words '%s' \n",mcroName);
                freeAllSplitString(splitLine,wordsCounter);
                free(sourceFile);
                fclose(inputFile);
                fclose(expandFile);
                freeMcroTable(macroTable);
                if(mcroName!=NULL){
                    free(mcroName);
                }
                if(line!=NULL){
                    free(line);
                }
                return False;
            }else {
                mcroTableInsert(macroTable, mcroName, "");
            }

        }else if (strcmp(splitLine[0], "endmcro") == 0){
            readMacroFlag = False;

        }else if(readMacroFlag == True){
            updatedMacro=allocateStr(mcroTableLockUp(macroTable,mcroName),line);
            mcroTableInsert(macroTable,mcroName, updatedMacro);
            free(updatedMacro);

        }else{
            fputs(line,expandFile);
        }

        freeAllSplitString(splitLine,wordsCounter);
        splitLine=NULL;
    }

    free(sourceFile);
    fclose(inputFile);
    fclose(expandFile);
    freeMcroTable(macroTable);
    if(mcroName!=NULL){
        free(mcroName);
    }
    if(line!=NULL){
        free(line);
    }
    return True;
}

char **splitStringLine(char *str,int *counter){
    int i=0,j=0,m;
    int strLength=strlen(str);
    int strI=0,I=0;
    char strSpace[]=" ";
    char *strCopy=NULL,**strSce=NULL,temp[MAX_LINE_LENGHT+2]={0};

    strCopy=(char*)malloc((1+strLength)*(sizeof(char)));
    if(strCopy==NULL){
        return NULL;
    }

    strncpy(strCopy,str,strLength+1);

    if(strCopy[strLength-1]=='\n' && strLength>0){
        strCopy[strLength-1]='\0';
        strLength--;
    }

    while(i<strLength){
        while(i<strLength){
            if(strchr(strSpace,strCopy[i])==NULL){
                break;
            }
            i++;
        }

        I=i;

        while(i<strLength){
            if(strchr(strSpace,strCopy[i])!=NULL){
                break;
            }
            i++;
        }

        if(i>I){
            *counter=*counter+1;
        }
    }

    strSce=(char**)malloc((*counter)*(sizeof(char*)));

    if(strSce==NULL){
        free(strCopy);
        return NULL;
    }

    i=0;
    while(i<strLength){
        while (i<strLength){
            if(strchr(strSpace,strCopy[i])==NULL){
                break;
            }
            i++;
        }

        j=0;
        while(i<strLength){
            if(strchr(strSpace,strCopy[i])!=NULL){
                break;
            }
            temp[j]=strCopy[i];
            i++;
            j++;
        }

        if(j>0){
            temp[j]='\0';
            strSce[strI]=(char*)malloc(sizeof(char)*(strlen(temp)+1));
            if(strSce[strI]==NULL){
                free(strCopy);
                for(m=0;m<strI;m++){
                    free(strSce[m]);
                }
                free(strSce);
                return NULL;
            }

            strcpy(strSce[strI++],temp);
        }
    }

    free(strCopy);

    return strSce;
}

void freeAllSplitString(char **splitString, int counter){
    int i;
    for(i=0;i<counter;i++){
        if(splitString[i]!=NULL)
            free(splitString[i]);
    }
    free(splitString);
}

