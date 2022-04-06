// Dependencies file
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "dep.h"

char* parse_command (char comm[], int s) {
	int t;
	char* m = "ola desde el server";
	if (! strcmp(comm,"mensaje_de_saludo")) {
		t = send(s, m, strlen(m),0);
		printf("dep: %d\n", t);
		return "some text";	
		//return SUCCESS_CODE " FTP Server v.1.0";
	}
	else if (! strcmp(comm,"quit")) {
        	//pclose(s);
		return GB_CODE " Goodbye";
	}
	else if (! strcmp(comm,"Contraseña Incorrecta")) {
		return FAILED_LOG_IN_CODE " Contraseña Incorrecta";
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
    return 0; //si no encuentra al user durante la lectua del archivo, entonces no esta
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
