#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 16

int rc;
int sock;

void on_exit(void) {
	rc = close(sock);
	if (rc == -1) {
		perror("close error");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Uzycie: %s <numer_portu>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
		perror("socket error");
		exit(EXIT_FAILURE);
	}

	rc = atexit(on_exit);
	if (rc != 0) {
		perror("atexit error");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_addr = { .s_addr = htonl(INADDR_ANY) },
		.sin_port = htons(atoi(argv[1]))
	};

	rc = bind(sock, (struct sockaddr *) & addr, sizeof(addr));
	if (rc == -1) {
		perror("bind error");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in klient;
	socklen_t size = sizeof(klient);
	unsigned char buffer[BUFFER_SIZE];
	char wizytowka[] = "Hello, world!\r\n";
	while (true) {
		rc = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) & klient, &size);
		if (rc == -1) {
			perror("recvfrom error");
			exit(EXIT_FAILURE);
		}

		rc = sendto(sock, wizytowka, sizeof(wizytowka), 0, (struct sockaddr *) & klient, size);
		if (rc == -1) {
			perror("sendto error");
			exit(EXIT_FAILURE);
		}
	}
}
