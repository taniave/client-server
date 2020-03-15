#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER "172.16.211.163"
#define PUERTO "6677"
#define PKTSIZE 1024

float operaciones(char operation[100]) {
	int i = 0, j = 0, k = 0, flag = 0;
	float num1, num2, ans;
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
	
	num1 = atof(n1);
	num2 = atof(n2);

	switch(opt) {
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
			printf("Division con cero. Operacion invalida.\n");
		}
		break;
	default:
		printf("No se puede resolver la operacion, intente mas tarde\n");
		break;
	}

	return ans;
}

int main(int argc, char *argv[]) {
	int sock_serv, sock_cli; // descriptores de sockets
	struct sockaddr_in socket_servidor,socket_cliente; //declaracion de estructuras;
	char operacion[PKTSIZE]; //cadena del cliente
	char resultado[PKTSIZE]; //cadena del servidor
	int inicio = 0; //determina el inicio de sesion
	char cs[PKTSIZE]; //cadena del servidor
	int bufs; //almacenamiento del tamanio cadena server
	int ciclo = 1; //variable para ciclo de lectura escritura
	int puerto; //variable para el puerto
	
	
 
	sock_serv = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_serv < 0){
        perror("ERROR EN LA CONEXION\n");
        exit(1);
    }
 
 	// se llena la estructura para el servidor con los datos necesarios
	socket_servidor.sin_family = AF_INET;
	socket_servidor.sin_addr.s_addr = inet_addr(SERVER);
	socket_servidor.sin_port = PUERTO;

	
	//avisamos al sistema operativo la creacion del socket
	if(bind(socket_servidor, (struct sockaddr *)&socket_servidor, sizeof(socket_servidor)) < 0){
        perror("ERROR AL CREAR EL BIND\n");
        exit(1);
    }

	//decimos al server que quede escuchando
	listen(sock_serv,5);

    int cliente_tam_socket = sizeof(struct sockaddr_in);
	printf("ESPERANDO PETICION...\n");
	sock_cli = accept(sock_serv, (struct sockaddr *)&socket_cliente,(socklen_t*)&cliente_tam_socket);
    if(sock_cli == -1){
        perror("FALLO AL ACEPTAR CONEXION\n");
        exit(1);
    }
    else{
        printf("--------CLIENTE CONECTADO------------\n");
    }

	while( valor = recv(socket_cliente, cs, PKTSIZE,0) > 0) {
        if(valor < 0)
            perror("ERROR AL RECIBIR LOS DATOS\n");
        else if(valor == 0)
            printf("CONEXION ACTIVA\n");
        else{
            printf("informacion recibida por el servidor\n");
            printf("OPERACION: %s",cs);
            printf("PROCESANDO...\n");
            if(cs[0] == '0' || cs[0] == '1' || cs[0] == '2' || cs[0] == '3' || cs[0] == '4' || cs[0] == '5' || cs[0] == '6' || cs[0] == '7' || cs[0] == '8' || cs[0] == '9') {
                float res = operaciones(cs);
                sprintf(operacion, "%f", res);
            }
            else {
                printf("Ingrese una cadena: \n");
                scanf("%*c%[^\n]", operacion);
            }
            send(socket_cliente,operacion,PKTSIZE,0);
        }
		

		close(sock_cli);
	}
	
    printf("~~~~~~~~~~~CONEXION PERDIDA CON EL SERVIDOR ~~~~~~~~~~~~~\n");

	close(sock_serv);

    return 0;
}