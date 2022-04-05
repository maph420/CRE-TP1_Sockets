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
