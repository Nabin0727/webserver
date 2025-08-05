// Building srver
//
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUFFSIZE 1000

int main()
{
	int socketServer = socket(AF_INET, SOCK_STREAM, 0);

	if(socketServer < 0){
		perror("Error creating Socket!!\n");
		exit(1);
	}
	
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = INADDR_ANY;

	if(bind(socketServer, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("Failed to bind socket\n");
		exit(1);
	}

	if(listen(socketServer, 10) < 0){
		perror("Failed to listen to socket\n");
		exit(1);
	}

	char buffer[BUFFSIZE];

	while(1)
	{
		printf("Waiting for connection.........\n");

		int addrLength = sizeof(address);
		int newSocket = accept(socketServer, (struct sockaddr *)&address, (socklen_t *)&addrLength);
		
		ssize_t bytes_received = recv(newSocket, buffer, sizeof(buffer), 0);

		if (bytes_received >= 0)
		{
			buffer[bytes_received]= '\0';

		}
		else{
			perror("Error reading buffer!");
		}

		char *response ="HTTP/1.1 200 OK\r\n"
                         	"Content-Type: text/html; charset=UTF-8\r\n\r\n"
                         	"<!DOCTYPE html>\r\n"
                         	"<html>\r\n"
                        	"<head>\r\n"
                        	"<title>Testing Basic HTTP-SERVER</title>\r\n"
                        	"</head>\r\n"
                        	"<body>\r\n"
                        	"Hello, From Server!!\r\n"
                        	"</body>\r\n"
                        	"</html>\r\n";
		send(newSocket, response, strlen(response), 0 );
		close(newSocket);

	}
	close(socketServer);

	return 0;
}
