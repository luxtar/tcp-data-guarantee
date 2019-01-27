#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
	int server_socket;
	int client_socket;
	int client_addr_size;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	char read_buf[BUFF_SIZE];
	char std_in[BUFF_SIZE];
	int rcv_buf = 16384;

	if (argc != 2)
	{
		printf("usage : %s [port]\n", argv[0]);
		exit(0);
	}

	memset(read_buf, 0x00, sizeof(read_buf));
	memset(std_in, 0x00, sizeof(std_in));

	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (-1 == server_socket) {
		printf("Fail created socket\n");
		exit(1);
	}

	int option = 1;
	int len = sizeof(rcv_buf);
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	setsockopt(server_socket, SOL_SOCKET, SO_RCVBUF, &rcv_buf, sizeof(rcv_buf)); // Set receiver buffer size
	getsockopt(server_socket, SOL_SOCKET, SO_SNDBUF, &rcv_buf, &len);
	printf("Server receive buffer size: %d\n", rcv_buf);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[1]));
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (-1 == bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
	{
		printf("Fail bind\n");
		exit(1);
	}

	if (-1 == listen(server_socket, 5))
	{
		printf("Fail listen\n");
		exit(1);
	}

	client_addr_size = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);

	if (-1 == client_socket) 
	{
		printf("Fail client accept\n");
		exit(1);
	}
	while (1)
	{
		fgets(std_in, sizeof(std_in), stdin);
		if (read(client_socket, read_buf, BUFF_SIZE) == 0)
		{
			close(client_socket);
		}
		printf("read: %s", read_buf);
	}

	return 0;
}