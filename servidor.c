#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define FIM "tchau"
struct sockaddr_in addrport, cl_addrport;


void criarConexaoTcpServidor(int* sockid, int* clsocket) {
	int status;
	socklen_t clntLen;
	*sockid = socket(PF_INET, SOCK_STREAM, 0);

	if (*sockid == -1) {
		printf("erro ao criar socket\n");
		exit(1);
	}
	printf("Socket criado\n");

	status = bind(*sockid, (struct sockaddr *) &addrport, sizeof(addrport));
	if(status == -1) {
		printf("erro ao dar o bind\n");
		exit(1);
	}
	printf("bind feito com sucesso\n");

	status = listen(*sockid, 1);
	if (status == -1) {
		printf("erro ao dar o listen\n");
		exit(1);
	}
	printf("Esperando por uma conexao\n");

	clntLen = sizeof(cl_addrport);
	*clsocket = accept(*sockid, (struct sockaddr *) &cl_addrport, &clntLen);
	if (*clsocket == -1) {
		printf("erro ao se conectar com o cliente\n");
		exit(1);
	}
	printf("conexao feita\n");
}

void criarConexaoUdpServidor(int* sockid, int* clsocket) {
	int status;
	*sockid = socket(PF_INET, SOCK_DGRAM, 0);

	if (*sockid == -1) {
		printf("erro ao criar socket\n");
		exit(1);
	}
	printf("Socket criado\n");

	status = bind(*sockid, (struct sockaddr *) &addrport, sizeof(addrport));
	if(status == -1) {
		printf("erro ao dar o bind\n");
		exit(1);
	}
	printf("bind feito com sucesso\n");
}

int FimServidorTcp(char* buffer, int* clsocket) {
	int status, flag=1;
	if (strcmp(buffer, FIM) == 0) {
		status = send(*clsocket, "Ate logo\0", sizeof("Ate logo\0"), 0);
		if (status == -1) {  
			printf("Erro ao enviar mensagem\n");  
		}
		flag = 0;
	}

	return flag;
}

int FimServidorUdp(char* buffer, int* sockid) {
	int status, flag=1;
	socklen_t clntLen;

	clntLen = sizeof(cl_addrport);
	if (strcmp(buffer, FIM) == 0) {
		status = sendto(*sockid, "Ate logo\0", sizeof("Ate logo\0"), 0, (struct sockaddr *) &cl_addrport, clntLen);
		if (status == -1) {  
			printf("Erro ao enviar mensagem\n");  
		}
		flag = 0;
	}

	return flag;
}

void mensagensTcpServidor(int* clsocket) {
	char buffer[1024];
	int status;

	while(1) {
		sleep(1);
		status = recv(*clsocket, buffer, sizeof(buffer), 0);
		if(status == -1) {
			printf("erro ao receber mensagem\n");
			exit(1);
		}
		printf("Mensagem do cliente: %s\n", buffer);

		if(FimServidorTcp(buffer, clsocket) == 0) {
			break;
		}
		memset(buffer, 0, sizeof(buffer));

		printf("Mensagem: ");
		scanf("%[^\n]s", buffer);
		getchar();
		status = send(*clsocket, buffer, sizeof(buffer), 0);  
		if (status == -1) {  
		   printf("Erro ao enviar mensagem\n");  
		}
		memset(buffer, 0, sizeof(buffer));
	}
}

void mensagensUdpServidor(int* sockid) {
	char buffer[1024];
	int status;
	socklen_t clntLen;

	clntLen = sizeof(cl_addrport);
	while(1) {
		sleep(1);
		status = recvfrom(*sockid, buffer, sizeof(buffer), 0, (struct sockaddr *) &cl_addrport, &clntLen);
		if(status == -1) {
			printf("erro ao receber mensagem\n");
			exit(1);
		}
		printf("Mensagem do cliente: %s\n", buffer);

		if(FimServidorUdp(buffer, sockid) == 0) {
			break;
		}
		memset(buffer, 0, sizeof(buffer));

		printf("Mensagem: ");
		scanf("%[^\n]s", buffer);
		getchar();
		status = sendto(*sockid, buffer, sizeof(buffer), 0, (struct sockaddr *) &cl_addrport, clntLen);  
		if (status == -1) {  
		   printf("Erro ao enviar mensagem\n");  
		}
		memset(buffer, 0, sizeof(buffer));
	}
}

void servidor(int tcp, int port) {
	int sockid, clsocket;

	memset(&addrport, 0, sizeof(addrport));
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(port);
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);


	if (tcp == 1) {
		criarConexaoTcpServidor(&sockid, &clsocket);
		mensagensTcpServidor(&clsocket);	
	}
	else {
		criarConexaoUdpServidor(&sockid, &clsocket);
		mensagensUdpServidor(&sockid);
	}


	close(sockid);
	close(clsocket);
}