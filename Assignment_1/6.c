#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int rc;
int plik_zrodlowy = -1;
int plik_docelowy = -1;

void on_exit(void) {
	if (plik_zrodlowy != -1) {
		rc = close(plik_zrodlowy);
		if (rc == -1) {
			perror("close error");
			exit(EXIT_FAILURE);
		}
	}
	if (plik_docelowy != -1) {
		rc = close(plik_docelowy);
		if (rc == -1) {
			perror("close error");
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Uzycie: %s <plik_zrodlowy> <plik_docelowy>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	rc = atexit(on_exit);
	if (rc != 0) {
		perror("atexit error");
		exit(EXIT_FAILURE);
	}

	plik_zrodlowy = open(argv[1], O_RDONLY);
	if (plik_zrodlowy == -1) {
		perror("open error");
		exit(EXIT_FAILURE);
	}
	plik_docelowy = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (plik_docelowy == -1) {
		perror("open error");
		exit(EXIT_FAILURE);
	}

	char buffer[BUFFER_SIZE];
	ssize_t bajty_przeczytane, bajty_zapisane;
	do {
		bajty_przeczytane = read(plik_zrodlowy, buffer, BUFFER_SIZE);
		if (bajty_przeczytane == -1) {
			perror("read error");
			exit(EXIT_FAILURE);
		}
		else if (bajty_przeczytane == 0) {
			break;
		}
		else if (bajty_przeczytane > 0) {
			bajty_zapisane = write(plik_docelowy, buffer, bajty_przeczytane);
			if (bajty_zapisane == -1) {
				perror("write error");
				exit(EXIT_FAILURE);
			}
		}
	}
	while (bajty_przeczytane > 0);
}
