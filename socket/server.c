#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "dep.h"

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


    memset(&h, 0, sizeof(h)); // se asegura de igualar a 0 cada caracter de h

    h.ai_family = AF_INET; //IPV4
    h.ai_socktype = SOCK_STREAM; //TCP
    h.ai_flags = AI_PASSIVE; // usar la ip del host actual

    status = getaddrinfo(NULL, argv[1], &h, &svinfo);

    // svinfo es una lista enlazada que contiene estructuras del tipo addrinfo

    if (status != 0) {
        fprintf(stderr, "error en la conexion: %s\n", gai_strerror(status));
        return -1;
    }

    sockfd = socket(svinfo->ai_family, svinfo->ai_socktype, svinfo->ai_protocol);

    // bindear el socket recien creado con la ip al puerto pasado al getaddrinfo
    bnd = bind(sockfd, svinfo->ai_addr, svinfo->ai_addrlen);

    if (bnd < 0) {
        fprintf(stderr,"Error durante el bindeo del socket: %s\n", gai_strerror(bnd));
        return -1;
    }

    // escuchar conexiones que lleguen
    listen(sockfd, BACKLOG);

    printf("Puerto %s abierto escuchando conexiones...\n", argv[1]);

    // aceptar conexion que llegue, a la cual se le asignara un nuevo socket especifico que se utilizara para enviar/recibir flujo

    addr_size = sizeof their_addr;
    sockstoragefd = accept(sockfd, (struct sockaddr*)&their_addr, &addr_size);
    int success_msg=1;

    while(1) {

        // recibir 
        int received_bytes;
        char buff[MAX_BUFF_LENGTH];
	char msg[MAX_BUFF_LENGTH];
        memset(buff,0,sizeof(buff)); //limpiar buffer por cada nuevo mensaje del host
	int t;
	char* response_sv;
        received_bytes = recv(sockstoragefd, buff, MAX_BUFF_LENGTH, 0);


            if (received_bytes < 0) {
                fprintf(stderr,"error: %s\n", gai_strerror(received_bytes));
                return -1;
            }

	    // el mensaje se entrego (sino ya habria retornado)
		printf("mensaje del cliente: %s\n", buff);

            if (success_msg == 1) { 
			    response_sv = parse_command(buff); // mostrar mensaje de recepcion, lo siguiente es pedir el user
			    t = send(sockstoragefd, response_sv,strlen(response_sv),0);	
                success_msg++;
            } else if (success_msg == 2) { 
                    response_sv = parse_command(buff); // recibe un username
	            	success_msg++;
                    //t = send(sockstoragefd, response_sv,strlen(response_sv),0);	
                    //guardar username en un char*
            } else if (success_msg == 3) { //recibe un password
                //con el username que guardamos, hay que llamar a una funcion que unifique el username con el password en una unica variable
                //luego, esa variable es pasada en check_user_and_pass() que devuelve 1 o 0
                //si devuelve 1 entonces manda a parse_command() un mensaje como "password_correcto" para que devuelva un codigo en base a ello, y el caso contrario si devuelve 0
                //t = send(sockstoragefd, response_sv,strlen(response_sv),0);
                success_msg++;	
            }
                
                else {
                    response_sv = parse_command(buff);
                    t = send(sockstoragefd, response_sv,strlen(response_sv),0);	
                    printf("host: %s\n", buff);
                    printf("response: %s\n",response_sv);
                    printf("%i", success_msg);
                    printf("bytes recibidos: %i\n", received_bytes);
                }
                
    }

    freeaddrinfo(svinfo); //importante liberar la memoria una vez terminada la lista enlazada
    return 0;

}
