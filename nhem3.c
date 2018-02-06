#include <stdio.h>
#include <string.h>

#include "servidor.c"
#include "cliente.c"

#define SERVIDOR "-S"
#define TCP "-t"

int main(int argc, char *argv[]) {
	int tcp=0, port=33333, i;
	if (argc < 2) {
  		printf("É necessario especificar  cliento ou servidor\n");
  		exit(1);
 	}
 
 	for (i=3;i<=argc;i++) {
 		if(strcmp(argv[i-1], TCP) == 0) {
 			tcp = 1;
 		}
 	}
 	
 	if(argc == 4) {
 		port = atoi( argv[2] );
 	}

 	if((argc == 3) && (tcp == 0)) {
 		port = atoi( argv[2] );
 	}


 	if (strcmp(argv[1], SERVIDOR) == 0) {
 		servidor(tcp, port);
 	}
 	else {
 		cliente(argv[1], tcp, port);
 	}

	return 0;
}