// Building srver

#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/sendfile.h>


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
		ssize_t bytes_received = recv(newSocket, buffer, sizeof(buffer)-1 , 0); // -1 at the buffer ensures there is no buffer overflow

		// validate the client request
		if (bytes_received >= 0)
		{
			buffer[bytes_received]= '\0'; // null terminator

		}
		else{
			perror("Error reading buffer!");
		
		}
	
		// print client request to console
		puts(buffer);

		// copy buffer to another variable
		char buffer1[BUFFSIZE];
		strcpy(buffer1, buffer);
		
		// parse the client request
		char *request = strtok(buffer1,"\r\n");
		char *method = strtok(request, " ");
		char *filename = strtok(NULL, " ");
		char *protocol = strtok(NULL, " ");
		
		if (!method || !filename || !protocol) {
    			fprintf(stderr, "Malformed request.\n");
    			close(newSocket);
    			continue;
		}

		if (strcmp(filename, "/") == 0)
			{
				char *response = "HTTP/1.1 200 OK\r\n"
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
			if (filename[0] == '/')
				filename++;
			
			// open file
			int opened_fd = open(filename, O_RDONLY);
			
			if (opened_fd < 0)
			{
				char *response = "HTTP/1.1 404 Error\r\n"
                	        	 	"Content-Type: text/html; charset=UTF-8\r\n\r\n"
                	        	 	"<!DOCTYPE html>\r\n"
                	        	 	"<html>\r\n"
                	        		"<head>\r\n"
                	        		"<title>File Not Found</title>\r\n"
                	        		"</head>\r\n"
                	        		"<body>\r\n"
                	        		"404 error\r\n"
                	        		"</body>\r\n"
                	        		"</html>\r\n";
				send(newSocket, response, strlen(response), 0 );
			}
			else 
			{
				struct stat st;
    				fstat(opened_fd, &st);
    				off_t file_size = st.st_size;
				
				char header[BUFFSIZE];
				
				/*snprintf(header, BUFFSIZE,
				         "HTTP/1.1 200 OK\r\n"
				         "Content-Type: text/html\r\n"
				         "Content-Length: %ld\r\n\r\n", st.st_size);*/
				/* char *response = "HTTP/1.1 200 OK\r\n"
                        	 	"Content-Type: text/html; charset=UTF-8\r\n\r\n"
                        	 	"<!DOCTYPE html>\r\n"
                        	 	"<html>\r\n"
                        		"<head>\r\n"
                        		"<title>This is index page!!</title>\r\n"
                        		"</head>\r\n"
                        		"<body>\r\n"
                        		"Hello, From Index Server!!\r\n"
                        		"</body>\r\n"
                        		"</html>\r\n";	
				*/
				// send response
				//send(newSocket, response, strlen(response), 0);		
				//send(newSocket, header, strlen(header), 0);		
				sendfile(newSocket, opened_fd, NULL, file_size);
			}
		}

		// close new socket
		close(newSocket);
	}

	// close orginal socket
	close(socketServer);
	return 0;
}
