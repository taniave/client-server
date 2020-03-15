#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

float operate(char operation[100]) {
	int i = 0, j = 0, k = 0, flag = 0;
	float number1, number2, answear;
	char n1[50], n2[50];
	char opt;

	while(operation[i] != '\0') {
		if(operation[i] == '+' || operation[i] == '-' || operation[i] == '*' || operation[i] == '/') {
			opt = operation[i];
			flag++;
		}
		else if(flag == 0 ) {
			n1[j] = operation[i];
			j++;
		}
		else {
			n2[k] = operation[i];
			k++;
		}
		i++;
	}
	
	number1 = atof(n1);
	number2 = atof(n2);

	switch(opt) {
	case '+':
		answear = number1 + number2;
		break;
	case '-':
		answear = number1 - number2;
		break;
	case '*':
		answear = number1 * number2;
		break;
	case '/':
		if(number2 != 0) {
			answear = number1 / number2;
		}
		else {
			printf("Division con cero. Operacion invalida.\n");
		}
		break;
	default:
		printf("No se puede resolver la operacion, intente mas tarde\n");
		break;
	}

	return answear;
}

main() {
	int server_sockfd, client_sockfd; // descriptores de sockets
	int server_len, client_len; //tamaños de las estructuras
	struct sockaddr_in server_address; //declaracion de estructuras
	struct sockaddr_in client_address;
	char c[1024]; //cadena del cliente
	char ch[1024]; //cadena del servidor
	int inicio = 0; //determina el inicio de sesion
	char cs[1024]; //cadena del servidor
	int bufs; //almacenamiento del tamanio cadena server
	int ciclo = 1; //variable para ciclo de lectura escritura
	int puerto; //variable para el puerto
	
	system("clear");
	printf("Introduzca el puerto de escucha: \n\n");
	scanf("%d",&puerto);
 
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
 
 	// se llena la estructura para el servidor con los datos necesarios
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("172.16.211.140");
	server_address.sin_port = puerto;
	server_len = sizeof(server_address);
	
	//avisamos al sistema operativo la creacion del socket
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

	//decimos al server que quede escuchando
	listen(server_sockfd,5);
	printf("SERVIDOR EN ESPERA...\n");
	
	while(ciclo) {
		//acepta la conexion con el cliente actual
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
		if(inicio ==0) {
			printf("------SESION INICIADA------\n");
			printf("CLIENTE CONECTADO\n");
			strcpy(ch,"SERVIDOR CONECTADO...");
			send(client_sockfd, ch, 1024,0);
			inicio = 1;
		}

		recv(client_sockfd, cs, 1024,0);
		printf("El cliente dijo: %s\n", cs);

		//if(cs[0] >= 48 && cs[0] <= 57) {
		if(cs[0] == '0' || cs[0] == '1' || cs[0] == '2' || cs[0] == '3' || cs[0] == '4' || cs[0] == '5' || cs[0] == '6' || cs[0] == '7' || cs[0] == '8' || cs[0] == '9') {
			float answear = operate(cs);
			sprintf(c, "%f", answear);
		}
		else {
			printf("Ingrese una cadena: \n");
			scanf("%*c%[^\n]", c);
		}
		send(client_sockfd, c, 1024,0);

		close(client_sockfd);
	}
	
	close(server_sockfd);
}