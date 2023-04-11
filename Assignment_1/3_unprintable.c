#include <stdio.h>
#include <stdbool.h>

bool printable_buf(const void * buf, int len) {
	const char *ptr = buf;
	const char *end = ptr + len;
	while (ptr < end) {
		if (*ptr < 32 || *ptr > 126)
			return false;
		ptr++;
	}
	return true;
}

int main(int argc, char *argv[]) {
	char buf[50];
	char ch;
	int i = 0;
	printf("Wprowadz znaki:\n");
	while (scanf("%c", &ch)) {
			if (ch - '0' == 0)
				break;
			if (i == 50)
				break;
			buf[i] = ch;
			i++;
	}

	if (printable_buf(buf, i))
		printf("Przekazany bufor zawiera tylko i wylacznie drukowalne znaki ASCII!\n");
	else
		printf("Przekazany bufor nie zawiera tylko i wylacznie drukowalnych znakow ASCII!\n");
}
