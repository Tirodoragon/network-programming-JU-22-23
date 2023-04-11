#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>

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
	if (argc != 3) {
		fprintf(stderr, "Uzycie: %s <adres> <numer_portu>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
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
		.sin_addr = { .s_addr = inet_addr(argv[1]) },
		.sin_port = htons(atoi(argv[2]))
	};

	rc = connect(sock, (struct sockaddr *) & addr, sizeof(addr));
	if (rc == -1) {
		perror("connect error");
		exit(EXIT_FAILURE);
	}

	unsigned char buffer[BUFFER_SIZE];

	ssize_t cnt = read(sock, buffer, BUFFER_SIZE);
	if (cnt == -1) {
		perror("read error");
		exit(EXIT_FAILURE);
	}

	buffer[cnt] = '\0';
	for (unsigned char *p = buffer; *p != '\0'; p++) {
		if (isprint(*p) || *p == '\n' || *p == '\r' || *p == '\t') {
			rc = putchar(*p);
		}
	}
	if (rc == EOF) {
		perror("putchar error");
		exit(EXIT_FAILURE);
	}	
}
