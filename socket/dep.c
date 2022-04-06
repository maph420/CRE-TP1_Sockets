// Dependencies file
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "dep.h"

char* parse_command (char comm[]) {
	if (! strcmp(comm,"mensaje_de_saludo")){	
		return SUCCESS_CODE " FTP Server v.1.0\n";
	}
	if (! strcmp(comm,"quit")) {
		return GB_CODE " Goodbye";
	}
	//if (comm.startsWith("user: ")) {
    //    return PW_REQ_CODE " Password required";
    //}

    //if (! strcmp(comm, "password_correcto")) {
      // return SUCCESS_LOG_IN_CODE " user logged in";
    //}

  //  if (!strmcmp(comm,"password_incorrect")) {
    //    return FAILED_LOG_IN_CODE " user not logged in";
    //}

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
