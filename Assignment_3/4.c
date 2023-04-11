#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <math.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#define NR_PORTU 2020
#define BUFFER_SIZE 65507

int rc;
int sock;
bool er = false;

void on_exit(void) {
    rc = close(sock);
    if (rc == -1) {
        perror("close error");
	exit(EXIT_FAILURE);
    }
}

void send_error(int sock, struct sockaddr_in klient, socklen_t size) {
    er = true;
    ssize_t cnt = sendto(sock, "ERROR", 5, 0, (struct sockaddr *) & klient, size);
    if (cnt == -1 || cnt != 5) {
        perror("sendto error");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
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
        .sin_port = htons(NR_PORTU)
    };

    rc = bind(sock, (struct sockaddr *) & addr, sizeof(addr));
    if (rc == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    	
    struct sockaddr_in klient;
    socklen_t size = sizeof(klient);
    while (true) {
        unsigned char buffer[BUFFER_SIZE];

        ssize_t cnt = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) & klient, &size);
        if (cnt == -1) {
            perror("recvfrom error");
            exit(EXIT_FAILURE);
        }

	char rd = 'o';
	int num_count = 0;
	char num[BUFFER_SIZE];
	er = true;
	long wn = 0;
	char oprc = 'n';
        errno = 0;
        for (int i = 0; i < cnt; i++) {
            if (buffer[i] == '+' && rd == 'n') {
                rd = 'o';
                num[num_count] = '\0';

                if (oprc == '-') {
                    long n = strtol(num, 0, 10);
                    if (errno != 0) {
                        send_error(sock, klient, size);
                        break;
                    }
                    if (wn < 0 && labs(LONG_MIN - wn) < n) {
                        send_error(sock, klient, size);
                        break;
                    }
                    else
                         wn -= n;
                }
                else if (oprc == '+') {
                    long n = strtol(num, 0, 10);
                    if (errno != 0) {
                        send_error(sock, klient, size);
                        break;
                    }
                    if (wn > 0 && LONG_MAX - wn < n) {
                        send_error(sock, klient, size);
                        break;
                    }
                    else
                        wn += n;
                }
                else if (oprc == 'n') {
                    wn = strtol(num, 0, 10);
                    if (errno != 0) {
                        send_error(sock, klient, size);
                        break;
                    }
                }

                num_count = 0;
                oprc = '+';
                er = true;
            }
            else if (buffer[i] == '-' && rd == 'n') {
                rd = 'o';
                num[num_count] = '\0';

                if (oprc == '-') {
                    long n = strtoll(num, 0, 10);
                    if (errno != 0){
                        send_error(sock, klient, size);
                        break;
                    }
                    if (wn < 0 && labs(LONG_MIN - wn) < n) {
                        send_error(sock, klient, size);
                        break;
                    }
                    else
                        wn -= n;
                }
                else if (oprc == '+') {
                    long n = strtol(num, 0, 10);
                    if (errno != 0) {
                        send_error(sock, klient, size);
                        break;
                    }
                    if (wn > 0 && LONG_MAX - wn < n) {
                        send_error(sock, klient, size);
                        break;
                    }
                    else
                        wn += n;
                }
                else if (oprc == 'n') {
                    wn = strtol(num, 0, 10);
                    if (errno != 0) {
                        send_error(sock, klient, size);
                        break;
                    }
                }

                num_count = 0;
                oprc = '-';
                er = true;
            }
            else if (buffer[i] >= '0' && buffer[i] <= '9' && rd != 'e') {
                er = false;
                rd = 'n';
                num[num_count] = buffer[i];
                num_count++;
            }
            else if (buffer[i] == '\r' && rd =='n') {
                rd = 'f';
                er = true;
            }
            else if (buffer[i] == '\n' && (rd == 'n' || rd == 'f')) {
                rd = 'e';
                er = false;
            }
            else {
                er = true;
                break;
            }
        }
        if (!er) {
            if (oprc == '-') {
                num[num_count] = '\0';
                long n = strtol(num, NULL, 10);
                if (errno != 0)
                    send_error(sock, klient, size);
                else if (wn < 0 && labs(LONG_MIN - wn) < n)
                    send_error(sock, klient, size);
                else
                    wn -= n;
            }
            else if (oprc == '+') {
                num[num_count] = '\0';
                long n = strtol(num, NULL, 10);
                if (errno != 0)
                    send_error(sock, klient, size);
                else if (wn > 0 && LONG_MAX - wn < n)
                    send_error(sock, klient, size);
                else
                    wn += n;
            }
            else if (oprc == 'n') {
                num[num_count] = '\0';
                wn = strtol(num, NULL, 10);
                if (errno != 0)
                    send_error(sock, klient, size);
            }
            if (!er) {
                if (rd == 'e') {
                    if (wn != 0) {
                        char odp[BUFFER_SIZE];
                        sprintf(odp, "%ld\r\n", wn);
                        cnt = sendto(sock, odp, strlen(odp), 0, (struct sockaddr *) & klient, size);
                        if (cnt == -1 || cnt != strlen(odp)) {
                            perror("sendto error");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else {
                        cnt = sendto(sock, "0\r\n", 3, 0, (struct sockaddr *) & klient, size);
                        if (cnt == -1 || cnt != 3) {
                            perror("sendto error");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                else {
                    if (wn != 0) {
                        char odp[BUFFER_SIZE];
                        sprintf(odp, "%ld", wn);
                        cnt = sendto(sock, odp, strlen(odp), 0, (struct sockaddr *) & klient, size);
                        if (cnt == -1 || cnt != strlen(odp)) {
                            perror("sendto error");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else {
                        cnt = sendto(sock, "0", 1, 0, (struct sockaddr *) & klient, size);
                        if (cnt == -1 || cnt != 1) {
                            perror("sendto error");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
            }
        }
        else {
            send_error(sock, klient, size);
        }
    }
}
