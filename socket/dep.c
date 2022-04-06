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
        pclose(s);
		return GB_CODE " Goodbye";
	}
	else if (! strcmp(comm,"Contraseña Incorrecta")) {
		return FAILED_LOG_IN_CODE " Contraseña Incorrecta";
    }
	else if (! strstr(comm,"Password required for user")) {
		return PW_REQ_CODE " Password required for user";
    }
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
