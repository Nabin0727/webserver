// Building srver
//
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>

#define BUFFSIZE 1000

int main()
{
	// lets initilize the socket
	int socketServer = socket(AF_INET, SOCK_STREAM, 0);

	if(socketServer < 0){
		perror("Error creating Socket!!\n");
		exit(1);
	}
	
	// address structure defination 
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = INADDR_ANY;

	// binding the socket
	if(bind(socketServer, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("Failed to bind socket\n");
		exit(1);
	}

	// listen to the socket
	if(listen(socketServer, 10) < 0){
		perror("Failed to listen to socket\n");
		exit(1);
	}

	// buffer to read the client request
	char buffer[BUFFSIZE];

	// loop starts
	while(1)
	{
		printf("Waiting for connection.........\n");

		// define address size
		int addrLength = sizeof(address);
		
		// accept the incomming connection
		int newSocket = accept(socketServer, (struct sockaddr *)&address, (socklen_t *)&addrLength);
		
		// receive the client request
		ssize_t bytes_received = recv(newSocket, buffer, sizeof(buffer), 0);

		// validate the client request
		if (bytes_received >= 0)
		{
			buffer[bytes_received]= '\0';

		}
		else{
			perror("Error reading buffer!");
		}
		
		// copy buffer to another variable
		char buffer1[BUFFSIZE];
		strcpy(buffer1, buffer);
		// parse the client request
		char *request = strtok(buffer1,"\r\n");
		char *request2 = strtok(request, " ");
		char *filename = strtok(NULL, " ");
		char *request4 = strtok(NULL, " ");

		// open file
		int opened_fd = open(filename, O_RDONLY);

		if (opened_fd < 0)
		{
			char *response = "HTTP/1.1 404 Not Found\r\n\r\n";
			send(newSocket, response, strlen(response), 0 );
		}
		else 
		{	
			if (file == "")
			{
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
		
				// send response to client
				send(newSocket, response, strlen(response), 0 );

			}
			else{
				fseek(opened_fd, 0, SEEK_END); // Move to end of file
    				long file_size = ftell(opened_fd); 
				
				// send avaliable file
				send(newSocket, opened_fd, file_size, 0);
			}{

		}


		// print client request to console
		puts(buffer);



		// close new socket
		close(newSocket);
		close(opened_fd);

	}

	// close orginal socket
	close(socketServer);

	return 0;
}
