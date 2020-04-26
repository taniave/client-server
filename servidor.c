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

float operaciones(char formula[100]) {
	int pos = 0, cont = 0, ban = 0, flag = 0;
	float num1, num2, ans;
	char n1[100], n2[100];
	char op;

	while(formula[pos] != '\0') {
		if(formula[pos] == '+' || formula[pos] == '-' || formula[pos] == '*' || formula[pos] == '/') {
			op = formula[pos];
			flag++;
		}
		else if(flag == 0 ) {
			n1[cont] = formula[pos];
			cont++;
		}
		else {
			n2[ban] = formula[pos];
			ban++;
		}
		pos++;
	}
	
	num1 = atof(n1);//convierte la cadena recibida a numero flotante
	num2 = atof(n2);

	//se realiza la operacion de acuerdo al signo que corresponde
	switch(op) {
	case '+':
		ans = num1 + num2;
		break;
	case '-':
		ans = num1 - num2;
		break;
	case '*':
		ans = num1 * num2;
		break;
	case '/':
		if(num2 != 0) {
			ans = num1 / num2;
		}
		else {
			printf("Division por cero.\n");
		}
		break;
	default:
		printf("No se puede resolver la operacion\n");
		break;
	}

	return ans;
}

int main(int argc, char *argv[]) {
	int sock_serv, sock_cli; // valor que describe al socket
	struct sockaddr_in socket_servidor,socket_cliente; //sockets
	char operacion[PKTSIZE]; //operacion enviada por el cliente
	char resultado[PKTSIZE]; //resultado de la operacion
	char cs[PKTSIZE]; //cadena del servidor
	int cliente_tam_socket // tamaño del socket del cliente
	
 
	sock_serv = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_serv < 0){
        perror("ERROR EN LA CONEXION\n");
        exit(1);
    }
 
 	// se llena el socket con los datos necesarios
	socket_servidor.sin_family = AF_INET;
	socket_servidor.sin_addr.s_addr = inet_addr(SERVER);
	socket_servidor.sin_port = htons(PUERTO);

	
	//Se avisa a ubuntu la creacion del socket
	if(bind(sock_serv, (struct sockaddr *)&socket_servidor, sizeof(socket_servidor)) < 0){
        perror("ERROR AL CREAR EL BIND\n");
        exit(1);
    }

	//El servidor se queda escuchando peticiones
	listen(sock_serv,5);

    cliente_tam_socket = sizeof(struct sockaddr_in);
	printf("ESPERANDO PETICION...\n");
	sock_cli = accept(sock_serv, (struct sockaddr *)&socket_cliente,(socklen_t*)&cliente_tam_socket);
    if(sock_cli == -1){
        perror("FALLO AL ACEPTAR CONEXION\n");
        exit(1);
    }
    else{
        printf("--------CLIENTE CONECTADO------------\n");
    }

	while( valor = recv(sock_cli, cs, PKTSIZE,0) > 0) {
        if(valor < 0)
            perror("ERROR AL RECIBIR LOS DATOS\n");
        else if(valor == 0)
            printf("CONEXION ACTIVA\n");
        else{
            printf("informacion recibida\n");
            printf("OPERACION: %s",cs);
            printf("PROCESANDO...\n");
            if(cs[0] == '0' || cs[0] == '1' || cs[0] == '2' || cs[0] == '3' || cs[0] == '4' || cs[0] == '5' || cs[0] == '6' || cs[0] == '7' || cs[0] == '8' || cs[0] == '9') {
                float res = operaciones(cs);
                sprintf(operacion,"%f", res);
            }
            else {
                printf("Ingrese una cadena: \n");
                scanf("%*c%[^\n]", operacion);
            }
            send(sock_cli,operacion,PKTSIZE,0);
        }
		

	
	}
	
    printf("~~~~~~~~~~~CONEXION PERDIDA CON EL SERVIDOR ~~~~~~~~~~~~~\n");
	close(sock_cli);
	close(sock_serv);

    return 0;
}