#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>

#define SERVER "172.16.211.166"
#define PUERTO 6677
#define PKTSIZE 1024


int main(int argc, char *argv[]){
    int enchufe,res;
    char respuesta[PKTSIZE];
    char operacion[PKTSIZE];

    struct sockaddr_in servidor; // se crea el socket que generara la conexion con el servidor

    system("clear");

        enchufe=socket(AF_INET, SOCK_STREAM,0);
        // se llena el socket con los datos necesarios
        servidor.sin_family = AF_INET;
        servidor.sin_addr.s_addr = inet_addr(SERVER); // se envia la direccion ip estatica del servidor
        servidor.sin_port = htons(PUERTO); // se envia el puerto por el que se realizara la conexion con una conversion de host byte order a network byte order

        printf("CONECTANDO CON EL SERVIDOR %s\n",SERVER); // imprime direccion ip del servidor

        res = connect(enchufe, (struct sockaddr *)&servidor, sizeof(servidor)); //se realiza la conexion
        if (res < 0){
            perror("ERROR EN LA CONEXION\n");
            close(enchufe);
            exit(1);
        }

        printf("~~~~~SESION DEL SERVIDOR INICIADA :D \n ~~~~~");

    while(1){
        
        printf("INGRESAR OPERACION A REALIZAR: \n");
        scanf("%*c%[^\n]",operacion); //evita que la cadena capture un salto de linea

        send(enchufe,operacion,PKTSIZE,0);//se envia la cadena de la operacion a realizar al servidor (se genera el paquete de informacion) 

        recv(enchufe,respuesta,PKTSIZE,0);//recibe la respuesta de la operacion relaizada por el servidor
        printf("RESPUESTA = %s\n",respuesta);

    }

    close(enchufe);

    return 0;
}