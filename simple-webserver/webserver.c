// Simple webserver in C
#include<sys/socket.h>
#include<string.h>
#include<fcntl.h>
#include<sys/sendfile.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdio.h>

int main()
{
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = INADDR_ANY;

	bind(server_socket, (struct sockaddr *)&addr, sizeof(addr));
	
	listen(server_socket, 10);

	printf("=== Webserver Running at 8080 ====\n");

	int client_fd = accept(server_socket, NULL, NULL);

	char buffer[256] = {0};

	recv(client_fd, buffer, 256, 0);
	
	// Basic parsing
	//GET /file.txt ....
	
	char *file = buffer + 5; // After "GET /"
	
	*strchr(file, ' ') = 0; // Trim after filename

	int opened_fd = open(file, O_RDONLY);

	if(opened_fd < 0){
		char *notfound = "HTTP/1.1 404 Not Found\r\n\r\n";
		send(client_fd, notfound, strlen(notfound), 0);
	}
	else{
		char *header = "HTTP/1.1 200 OK\r\n\r\n";
		send(client_fd, header, strlen(header), 0);
		sendfile(client_fd, opened_fd, NULL, 256);
		close(opened_fd);
	}

	close(client_fd);
	close(server_socket);

	return 0;
}
