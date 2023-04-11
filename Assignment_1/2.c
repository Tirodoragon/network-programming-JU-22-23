#include <stdio.h>

void drukuj_alt(int * tablica, int liczba_elementow) {
	int *ptr = tablica;
	printf("Liczby wieksze od 10 i mniejsze od 100:\n");
	while (ptr < tablica + liczba_elementow) {
		if (*ptr > 10 && *ptr < 100)
			printf("%d\n", *ptr);
		ptr++;
	}
}

int main(int argc, char *argv[]) {
	int liczby[50];
	int liczba;
	int i = 0;
	printf("Wprowadz liczby:\n");
	while (scanf("%d", &liczba)) {
		if (liczba == 0)
			break;
		if (i == 50)
			break;
		liczby[i] = liczba;
		i++;
	}

	drukuj_alt(liczby, i);

	return 0;
}
