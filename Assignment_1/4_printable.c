#include <stdio.h>
#include <stdbool.h>

bool printable_str(const char * buf) {
	while (*buf != '\0') {
		if (*buf < 32 || *buf > 126)
			return false;
		buf++;
	}
	return true;
}

int main(int argc, char *argv[]) {
	char buf[50];
	printf("Wprowadz lancuch znakow:\n");
	scanf("%49s", buf);

	if (printable_str(buf))
		printf("Przekazany bufor zawiera tylko i wylacznie drukowalne znaki ASCII!\n");
	else
		printf("Przekazany bufor nie zawiera tylko i wylacznie drukowalnych znakow ASCII!\n");
}
