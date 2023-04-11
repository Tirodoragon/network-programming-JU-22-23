#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int rc;
int lst_sock = -1;
int clnt_sock = -1;

void on_exit(void) {
	if (lst_sock != -1) {
		rc = close(lst_sock);
		if (rc == -1) {
			perror("close error");
			exit(EXIT_FAILURE);
		}
	}
	if (clnt_sock != -1) {
		rc = close(clnt_sock);
		if (rc == -1) {
			perror("close error");
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Uzycie: %s <numer_portu>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	rc = atexit(on_exit);
	if (rc != 0) {
		perror("atexit error");
		exit(EXIT_FAILURE);
	}

	lst_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (lst_sock == -1) {
		perror("socket error");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_addr = { .s_addr = htonl(INADDR_ANY) },
		.sin_port = htons(atoi(argv[1]))
	};

	rc = bind(lst_sock, (struct sockaddr *) & addr, sizeof(addr));
	if (rc == -1) {
		perror("bind error");
		exit(EXIT_FAILURE);
	}

	rc = listen(lst_sock, 10);
	if (rc == -1) {
		perror("listen error");
		exit(EXIT_FAILURE);
	}

	bool keep_on_handling_clients = true;
	char wizytowka[] = "Hello, world!\r\n";
	ssize_t cnt;
	while (keep_on_handling_clients) {
		clnt_sock = accept(lst_sock, NULL, 0);
		if (clnt_sock == -1) {
			perror("accept error");
			exit(EXIT_FAILURE);
		}
		
		cnt = write(clnt_sock, wizytowka, sizeof(wizytowka));
		if (cnt == -1) {
			perror("write error");
			exit(EXIT_FAILURE);
		}

		rc = close(clnt_sock);
		if (rc == -1) {
			perror("close error");
			exit(EXIT_FAILURE);
		}
	}

	clnt_sock = -1;
}
