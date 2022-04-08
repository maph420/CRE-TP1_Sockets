#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "dep.h"

int main (int argc, char* argv[]) {

    if (argc != 3) {
        fprintf(stderr,"No se recibieron la cantidad correcta de argumentos\nUso: ./client <dir ip del servidor> <puerto del servidor>\n");
        exit(1);
    }

    printf("IP del servidor: %s\n",argv[1]);
    printf("Puerto del servidor: %s\n",argv[2]);

    int status, sockfd, conn;
    struct addrinfo h;
    struct addrinfo *svinfo;

    memset(&h, 0, sizeof(h));
    h.ai_family = AF_INET; //IPV4
    h.ai_socktype = SOCK_STREAM; //TCP

    status = getaddrinfo(argv[1], argv[2], &h, &svinfo);

    if (status < 0) {
        fprintf(stderr,"Error: %s\n", gai_strerror(status));
        return -1;
    }

    // svinfo es una lista enlazada que contiene estructuras del tipo addrinfo

    sockfd = socket(svinfo->ai_family, svinfo->ai_socktype, svinfo->ai_protocol);


    conn = connect(sockfd, svinfo->ai_addr, svinfo->ai_addrlen);

    if (conn == -1) {
        fprintf(stderr,"Error al conectar con el socket del servidor, ¿el programa del servidor esta corriendo? (%s)\n", gai_strerror(conn));
        return -1;
    }

    char msg[MAX_BUFF_LENGTH];
    int bytes_sent, init=1;
    int r;
    char buff[MAX_BUFF_LENGTH];

    while(1) {
	
	memset(buff,0,sizeof(buff));
	memset(buff,0,sizeof(msg));
        // la funcion de init es detectar la primera interaccion cliente->servidor
        if (init == 1) {
            strcpy(msg,"mensaje_de_saludo");
            init++;
        } else if(init == 2) {
            printf(">: ");
            scanf("%[^\n]",msg), getchar();
	        init++;
        } else if(init == 3) {
            printf(">: ");
            scanf("%[^\n]",msg), getchar();
            init++;
	} else {
            memset(msg,0,sizeof(msg));
            printf(">: ");
            scanf("%[^\n]%*c", msg);
        }

        bytes_sent = send(sockfd, msg, strlen(msg),0);

        if (bytes_sent < 0) {
            fprintf(stderr,"Error al enviar el mensaje: %s\n", gai_strerror(bytes_sent));
            return -1;
        } else {
            	/* printf("Mensaje: %s (bytes enviados: %i)\n", msg, bytes_sent); */
	        r = recv(sockfd,buff,MAX_BUFF_LENGTH,0);
	        if (r > 0) printf("server: %s\n", buff);
	        else fprintf(stderr,"Mensaje no enviado (%s)\n",gai_strerror(r));
        }

    }
    // liberar memoria
    freeaddrinfo(svinfo);
    return 0;

}
