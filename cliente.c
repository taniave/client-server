#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER "172.16.211.163"
#define PUERTO "6677"
#define PKTSIZE 1024


int main(int argc, char *argv[]){
    int enchufe,len,res,buffer,inicio_server=0,buff;
    char respuesta[PKTSIZE];
    char operacion[PKTSIZE];

    struct sockaddr_in servidor;

    system("clear");

        enchufe=socket(AF_INET, SOCK_STREAM,0);
        servidor.sin_family = AF_INET;
        servidor.sin_addr.s_addr = inet_addr(SERVER);
        servidor.sin_port = PUERTO;

        printf("CONECTANDO CON EL SERVIDOR %s\n",SERVER);
        result = connect(enchufe, (struct sockaddr *)&servidor, sizeof(servidor));
        if (result < 0){
            perror("ERROR EN LA CONEXION\n");
            close(enchufe);
            exit(1);
        }

        printf("SESION DEL SERVIDOR INICIADA :D \n\n\n\n");

       /* if(inicio_server == 0){
            printf("SESION DEL SERVIDOR INICIADA\n");
            recv(enchufe,respuesta,PKTSIZE,0);
            printf("%s\n",respuesta);
            inicio_server = 1;
        }*/


    while(1){
        
        printf("INGRESAR OPERACION A REALIZAR: \n");
        scanf("%*c%[^\n]",operacion);
        send(enchufe,operacion,PKTSIZE,0);

        recv(enchufe,respuesta,PKTSIZE,0);
        printf("%s\n",respuesta);

    }

    close(enchufe);

    return 0;
}