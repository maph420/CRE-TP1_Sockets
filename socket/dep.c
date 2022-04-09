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

	if (! strcmp(comm,"mensaje_de_saludo")) {	
		return SUCCESS_CODE " FTP Server v.1.0\n";
	}
	if (! strcmp(comm,"quit")) {
		return GB_CODE " Goodbye";
	}
	if (! strncmp(comm, "user: ",6) || ! strncmp(comm, "user:",5)) {
        	return PW_REQ_CODE " Password required";
    	}
    	if (! strcmp(comm, "password_correcto")) {
       		return SUCCESS_LOG_IN_CODE " user logged in";
    	}
    	if (!strcmp(comm,"password_incorrecto")) {
        return FAILED_LOG_IN_CODE " user not logged in";
   	}
	return " "; //default
}

 
int check_user(char* path, char user[]) {

	char line[255];
	FILE* arch = fopen(path,"r");

	if (!arch) {       
		fprintf(stderr,"Hubo un problema al leer el archivo\n");
		return -1;
	}
	// int i=0;
	while(fgets(line,255,arch)) {
		 /*printf("Linea %d: %s\n",++i,line);
		printf("Comparo %s y %s\n",line,user);*/
		if (! strcmp(line,user)) {
			/*printf("Match encontrado! (%s y %s\n)", line,user);*/
			fclose(arch);
			return 1;	
		}
	}
	fclose(arch);
	return 0;
}


void merge_user_data (char name[], char password[], char dst[]) {
	char* nameExtracted = strtok(name,": ");
	nameExtracted = strtok(NULL,": ");
	
	char* passExtracted = strtok(password,": ");
	passExtracted = strtok(NULL,": ");
	
	
	snprintf(dst, MAX_BUFF_LENGTH,"%s:%s\n",nameExtracted,passExtracted);
}


