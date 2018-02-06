#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define FIM "tchau"
struct sockaddr_in addrport;

void criarConexaoTcpCliente(int* sockid) {
	int status;
	*sockid = socket(PF_INET, SOCK_STREAM, 0);

	if (*sockid == -1) {
		printf("erro ao criar socket\n");
		exit(1);
	}
	printf("Socket criado\n");

	status = connect(*sockid, (struct sockaddr *) &addrport, sizeof(addrport));
	if (status == -1) {
		printf("error ao criar conexao\n");
		exit(1);
	}
	printf("cliente conectado\n");
}

void criarConexaoUdpCliente(int* sockid) {
	*sockid = socket(PF_INET, SOCK_DGRAM, 0);
	if (*sockid == -1) {
		printf("erro ao criar socket\n");
		exit(1);
	}
	printf("Socket criado\n");



}

int FimClienteTcp(char* buffer, int* sockid) {
	int status, flag=1;
	if (strcmp(buffer, FIM) == 0) {
		sleep(1);
		status = recv(*sockid, buffer, sizeof(buffer), 0);
		if(status == -1) {
			printf("erro ao receber mensagem\n");
			exit(1);
		}
		printf("Mensagem do servidor: %s\n", buffer);
		flag = 0;
	}

	return flag;
}

int FimClienteUdp(char* buffer, int* sockid) {
	int status, flag=1;
	socklen_t len;
	if (strcmp(buffer, FIM) == 0) {
		sleep(1);
		len = sizeof(addrport);
		status = recvfrom(*sockid, buffer, sizeof(buffer), 0, (struct sockaddr *) &addrport, &len);
		if(status == -1) {
			printf("erro ao receber mensagem\n");
			exit(1);
		}
		printf("Mensagem do servidor: %s\n", buffer);
		flag = 0;
	}

	return flag;
}

void mensagensTcpCliente(int* sockid) {
	int status;
	char buffer[1024];
	while(1) {
		printf("Mensagem: ");
		scanf("%[^\n]s", buffer);
		getchar();
		status = send(*sockid, buffer, sizeof(buffer), 0);  
		if (status == -1) {  
		   printf("Erro ao enviar mensagem\n");  
		}

		if(FimClienteTcp(buffer, sockid) == 0){
			break;
		}
		memset(buffer, 0, sizeof(buffer));

		sleep(1);
		status = recv(*sockid, buffer, sizeof(buffer), 0);
		if(status == -1) {
			printf("erro ao receber mensagem\n");
			exit(1);
		}
		printf("Mensagem do servidor: %s\n", buffer);
		memset(buffer, 0, sizeof(buffer));
	}
}

void mensagensUdpCliente(int* sockid) {
	int status;
	socklen_t len;
	char buffer[1024];
	while(1) {
		printf("Mensagem: ");
		scanf("%[^\n]s", buffer);
		getchar();
		status = sendto(*sockid, buffer, sizeof(buffer), 0, (struct sockaddr *) &addrport, sizeof(addrport));  
		if (status == -1) {  
		   printf("Erro ao enviar mensagem\n");  
		}

		if(FimClienteUdp(buffer, sockid) == 0){
			break;
		}
		memset(buffer, 0, sizeof(buffer));

		sleep(1);
		len = sizeof(addrport);
		status = recvfrom(*sockid, buffer, sizeof(buffer), 0, (struct sockaddr *) &addrport, &len);
		if(status == -1) {
			printf("erro ao receber mensagem\n");
			exit(1);
		}
		printf("Mensagem do servidor: %s\n", buffer);
		memset(buffer, 0, sizeof(buffer));
	}
}


void cliente(char* ip, int tcp, int port) {
	int sockid;

	memset(&addrport, 0, sizeof(addrport));
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(port);
	addrport.sin_addr.s_addr = inet_addr(ip);

	if (tcp == 1) {
		criarConexaoTcpCliente(&sockid);
		mensagensTcpCliente(&sockid);
	}
	else {
		criarConexaoUdpCliente(&sockid);
		mensagensUdpCliente(&sockid);
	}

	close(sockid);
}