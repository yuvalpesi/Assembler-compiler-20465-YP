
#define MISSING_PARAMITER(ch,loc,error,lineNode) \
  if(!ch){ \
    fprintf(stderr,"Missing parameter in line number %d\n",loc); \
    error++;                                 \
    if(lineNode!=NULL)    \
     lineNode=lineNode->next;   \
    continue;\
  }

#define MISSING_PARAMITER_DATA(ch,loc,error,i,lineNode) \
  if(!ch){ \
    fprintf(stderr,"Missing parameter in line number %d\n",loc); \
    error++;    \
    i++;   \
    if(lineNode!=NULL)    \
     lineNode=lineNode->next;   \
    continue;\
  }

#define EXTRANEOUS_TEXT_AFTER_END_OF_COMMAND(ch,loc,error,i,lineNode) \
  if(ch){ \
    fprintf(stderr,"Extraneous text after end of command in line number %d \n",loc);     \
    error++;    \
    i++;   \
    if(lineNode!=NULL)    \
     lineNode=lineNode->next;   \
    continue;\
  }

#define ILLEGAL_COMMA(ch,loc,error,lineNode) \
  if(ch==','){ \
    fprintf(stderr,"Illegal comma in line number %d \n",loc); \
    error++;                                 \
    if(lineNode!=NULL)    \
     lineNode=lineNode->next;   \
    continue;\
  }

#define MULIPLE_CONSECUTIVE_COMMAS(ch,loc,error,lineNode) \
  if(ch == ','){ \
    fprintf(stderr,"Multiple consecutive commas in line number %d \n",loc); \
    error++;                                 \
    if(lineNode!=NULL)    \
     lineNode=lineNode->next;   \
    continue;\
  }

#define MULIPLE_CONSECUTIVE_COMMAS_DATA(ch,loc,error,i,lineNode) \
  if(ch == ','){ \
    fprintf(stderr,"Multiple consecutive commas in line number %d \n",loc); \
    error++;                                 \
    if(lineNode!=NULL)    \
     lineNode=lineNode->next;   \
    continue;\
    continue; \
  }

#define MISSING_COMMA(ch,loc,error,lineNode) \
  if(ch!=','){ \
    fprintf(stderr,"Missing comma in line number %d \n",loc); \
    error++;                                 \
    if(lineNode!=NULL)    \
     lineNode=lineNode->next;   \
    continue;\
  }

#define UNDEFINED_REGISTER_NUMBER(reg,loc,error,lineNode) \
  if(reg==-1){ \
    fprintf(stderr,"Undefined register number in line number %d \n",loc); \
    error++;                                 \
    if(lineNode!=NULL)    \
     lineNode=lineNode->next;   \
    continue;\
  }

#define TOO_LONG_LINE(ch,loc,error,lineNode) \
  if(strlen(ch)>MAX_LINE_LENGHT){                 \
    fprintf(stderr,"Too long line %s, can not pass 80 chars in line number %d \n",ch,loc);  \
    error++;                                 \
    if(lineNode!=NULL)    \
     lineNode=lineNode->next;   \
    continue;\
  }
