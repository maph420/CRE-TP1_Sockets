// Dependencies file
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "dep.h"

char* parse_command (char comm[], int s) {
	if (! strcmp(comm,"mensaje_de_saludo")) {
		return SUCCESS_CODE " FTP Server v.1.0";
	}
	else if (! strcmp(comm,"quit")) {
		//aca falta cerrar el socket
		return GB_CODE " Goodbye";
	}
	else
		return " ";
}

int check_user(char* path, char* user) {
    FILE* fp = fopen(path, "r");
    char* delim = ":";
    char str[999];
    if (fp) {
        while (fscanf(fp, "%s", str)!=EOF){
            strtok(str, delim);
            if(strstr(str, user)) return 1;
        }
        fclose(fp);
    }
    return 0;
}

int check_paswwd(char* path, char* passwd) {
    FILE* fp = fopen(path, "r");
    char* delim = ":";
    char str[999];
    if (fp) {
        while (fscanf(fp, "%s", str)!=EOF){
            strtok(str, delim);
            strtok(NULL, delim);
            if(strstr(str, passwd)) return 1;
        }
        fclose(fp);
    }
    return 0;
}
