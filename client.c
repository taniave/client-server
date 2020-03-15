#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

main() {
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch[1024];
	char c[1024];
	int buf;
	int inicio = 0;
	char cs[1024];
	int bufs;
	int ciclo =1;
	char ipserver[10];
	int puerto;
	
	system("clear");

	printf("Ingrese la IP del servidor\n");
	scanf("%s",ipserver);

	printf("Ingrese el puerto de conexi�n\n");
	scanf("%d",&puerto);

	while(ciclo) {
		sockfd = socket(AF_INET, SOCK_STREAM,0);
		
		//llenado de la estructura de datos
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr(ipserver);
		address.sin_port = puerto;
		len = sizeof(address);

		//conectar con el server
		result = connect(sockfd, (struct sockaddr *)&address, len);
		if(result ==-1) {
			perror("ERROR EN LA CONEXION\n");
			close(sockfd);
		}

		//validar el inicio de sesion
		if(inicio==0) {
			printf("--------------- SESION INICIADA --------------\n");
			recv(sockfd, ch, 1024,0);
			printf("%s\n",ch);
			inicio = 1;
		}

		printf("INGRESAR LA OPERACION QUE DESEA REALIZAR: ");
		scanf("%*c%[^\n]",c);
		send(sockfd,c, 1024,0);

		recv(sockfd, ch, 1024,0);
		printf("El servidor dijo: %s\n",ch);
	}

	close(sockfd);
}