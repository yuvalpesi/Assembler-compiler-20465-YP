
#define INVALID_PARAMETER_NOT_A_NUMBER(ch1,ch2,loc,error,i) \
  if(ch1!='-' || !isdigit(ch2) ){ \
    fprintf(stderr,"Invalid parameter - not a number '%c' in line number %d \n",ch1,loc);        \
    error++;    \
    i++;   \
    linestr=linestr->next;   \
    continue; \
  }

#define MISSING_PARAMITER(ch,loc,error) \
  if(!ch){ \
    fprintf(stderr,"Missing parameter in line number %d\n",loc); \
    error++;                        \
    linestr=linestr->next;   \
    continue;\
  }

#define MISSING_PARAMITER_DATA(ch,loc,error,i) \
  if(!ch){ \
    fprintf(stderr,"Missing parameter in line number %d\n",loc); \
    error++;    \
    i++;   \
    linestr=linestr->next;   \
    continue; \
  }

#define EXTRANEOUS_TEXT_AFTER_END_OF_COMMAND(ch,loc,error,i) \
  if(ch){ \
    fprintf(stderr,"Extraneous text after end of command in line number %d \n",loc);     \
    error++;    \
    i++;   \
    linestr=linestr->next;   \
    continue; \
  }

#define ILLEGAL_COMMA(ch,loc,error) \
  if(ch==','){ \
    fprintf(stderr,"Illegal comma in line number %d \n",loc); \
    error++;                        \
    linestr=linestr->next;   \
    continue;\
  }

#define MULIPLE_CONSECUTIVE_COMMAS(ch,loc,error) \
  if(ch == ','){ \
    fprintf(stderr,"Multiple consecutive commas in line number %d \n",loc); \
    error++;                        \
    linestr=linestr->next;   \
    continue;\
  }

#define MULIPLE_CONSECUTIVE_COMMAS_DATA(ch,loc,error,i) \
  if(ch == ','){ \
    fprintf(stderr,"Multiple consecutive commas in line number %d \n",loc); \
    error++;    \
    i++;   \
    linestr=linestr->next;   \
    continue; \
  }

#define MISSING_COMMA(ch,loc,error) \
  if(ch!=','){ \
    fprintf(stderr,"Missing comma in line number %d \n",loc); \
    error++;                        \
    linestr=linestr->next;   \
    continue;\
  }

#define UNDEFINED_REGISTER_NUMBER(reg,loc,error) \
  if(reg==-1){ \
    fprintf(stderr,"Undefined register number in line number %d \n",loc); \
    error++;                        \
    linestr=linestr->next;   \
    continue;\
  }

#define TOO_LONG_LINE(ch,loc,error) \
  if(strlen(ch)>MAX_LINE_LENGHT){                 \
    fprintf(stderr,"Too long line %s, can not pass 80 chars in line number %d \n",ch,loc);  \
    error++;                        \
    linestr=linestr->next;   \
    continue;\
  }
