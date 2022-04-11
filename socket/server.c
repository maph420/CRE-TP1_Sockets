#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "dep.h"

// esto es una linea de codigo comentada
/* esto es un comentario */

int main (int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr,"No se recibieron la cantidad correcta de argumentos\nUso: ./server <puerto de donde se escuchara al cliente>\n");
        exit(1);
    }

    int status, sockfd, sockstoragefd, bnd;
    struct addrinfo h;
    struct addrinfo *svinfo;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;


    memset(&h, 0, sizeof(h)); /* se asegura de igualar a 0 cada caracter de h (basicamente, limpia el buffer) */

    h.ai_family = AF_INET; /*IPV4*/
    h.ai_socktype = SOCK_STREAM; /*TCP*/
    h.ai_flags = AI_PASSIVE; /*usar la ip del host actual*/

    status = getaddrinfo(NULL, argv[1], &h, &svinfo);

    /* svinfo es una lista enlazada que contiene estructuras del tipo addrinfo */

    if (status != 0) {
        fprintf(stderr, "error en la conexion: %s\n", gai_strerror(status));
        return -1;
    }

    sockfd = socket(svinfo->ai_family, svinfo->ai_socktype, svinfo->ai_protocol);

    /* bindear el socket recien creado con la ip al puerto pasado al getaddrinfo */
    bnd = bind(sockfd, svinfo->ai_addr, svinfo->ai_addrlen);

    if (bnd < 0) {
        fprintf(stderr,"Error durante el bindeo del socket: %s\n", gai_strerror(bnd));
        return -1;
    }

    /* escuchar conexiones que lleguen */
    listen(sockfd, BACKLOG);

    printf("Puerto %s abierto escuchando conexiones...\n", argv[1]);

    /* aceptar conexion que llegue, a la cual se le asignara un nuevo socket especifico que se utilizara para enviar/recibir flujo */

    addr_size = sizeof their_addr;
    sockstoragefd = accept(sockfd, (struct sockaddr*)&their_addr, &addr_size);
    int success_msg=1;

        /* recibir */ 
        int received_bytes, t, off=0;
        char buff[MAX_BUFF_LENGTH], msg[MAX_BUFF_LENGTH], user[MAX_BUFF_LENGTH], dst[MAX_BUFF_LENGTH];
	char* response_sv;

	while (1) {
	
	/* asegurar que los buffer de entrada y salida esten limpios */
        memset(buff,0,sizeof(buff));
	memset(buff,0,sizeof(msg));

        received_bytes = recv(sockstoragefd, buff, MAX_BUFF_LENGTH, 0);
	// printf("success_msg: %i ,buff:%s\n",success_msg,buff);
		
            if (received_bytes < 0) {
                fprintf(stderr,"error: %s\n", gai_strerror(received_bytes));
                return -1;
            }
		//printf("mensaje recibido desde el cliente: %s\n", buff); 

            if (success_msg == 1) { 
		response_sv = parse_command(buff); /* cargar mensaje de recepcion */	
               	success_msg++;
            } else if (success_msg == 2) { 
            	response_sv = parse_command(buff); /* cargar user */
	       	success_msg++;
		strcpy(user,buff); //guardar username
            } else if (success_msg == 3) { 
		response_sv = parse_command(buff); /* cargar password */
		merge_user_data (user,buff,dst);
		
		// printf("Par a verificar: %s\n",dst);	
		
		if (check_user("ftpusers.txt",dst)) {
			strcpy(buff,"password_correcto");
			success_msg++;
		}
		else {
			strcpy(buff,"password_incorrecto");
			success_msg-=2;
		}
		response_sv = parse_command(buff);	
            }
                
                else {
			if (off) shutdown(sockstoragefd, SHUT_RDWR);
				 
			response_sv = parse_command(buff);
			
			if (! strncmp(response_sv,"221",3)) {
				off=1;
			}
		}	    
	
		if (strcmp(response_sv," ") || strncmp(buff, "pass: ",8)) { 
			printf("%s\n",response_sv);
		} else printf("Texto recibido desde el cliente (no es un comando): %s\n", buff);
		

		t = send(sockstoragefd, response_sv, strlen(response_sv),0);
		
               	if (t<0) {
			fprintf(stderr,"Mensaje no enviado (%s)\n",gai_strerror(t));	
    		}
	}
	
/* liberar memoria una vez finalizado el programa */
    freeaddrinfo(svinfo); 
    return 0;

}
