#ifndef __DEP_H__
#define __DEP_H__
#include <stddef.h>
#define BACKLOG 5 //cantidad maxima de conexiones apilables a escuchar
#define MAX_BUFF_LENGTH 255 //cantidad maxima de caracteres admitidos entre cada envio de mensajes

#define SUCCESS_CODE "220"
#define PW_REQ_CODE "331"
#define SUCCESS_LOG_IN_CODE "230"
#define FAILED_LOG_IN_CODE "530" 
#define GB_CODE "221"

int foo (int c);

char* parse_command(char comm[]);

int check_user(char* path, char* user);

void merge_user_data (char name[], char password[], char* dst);


#endif
