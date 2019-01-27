#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFF_SIZE 1024

int main(int argc, char **argv)
{
	struct sockaddr_in serveraddr;
	int server_socket;
	int client_len;
	char write_buf[BUFF_SIZE];

	if (argc != 3)
	{
		printf("usage : %s ip_Address port\n", argv[0]);
		exit(0);
	}

	memset(write_buf, 0x00, sizeof(write_buf));

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Error : ");
		exit(1);
	}

	int snd_buf = 8192;
	int option = 1;
	int len = sizeof(snd_buf);

	setsockopt(server_socket, SOL_SOCKET, SO_SNDBUF, &snd_buf, sizeof(snd_buf));    // Set sender buffer size
	getsockopt(server_socket, SOL_SOCKET, SO_SNDBUF, &snd_buf, &len);
	printf("Client send buffer size: %d\n", snd_buf);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	client_len = sizeof(serveraddr);

	if (connect(server_socket, (struct sockaddr*)&serveraddr, client_len) == -1)
	{
		perror("Error connect : ");
		exit(1);
	}

	while (1)
	{
		printf("write: ");
		fgets(write_buf, sizeof(write_buf), stdin);
		write_buf[strlen(write_buf) - 1] = '\0';

		if (write(server_socket, write_buf, BUFF_SIZE) >= 0)
		{
			close(server_socket);
			break;
		}
	}

	return 0;
}