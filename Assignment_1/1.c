#include <stdio.h>

void drukuj(int tablica[], int liczba_elementow) {
	printf("Liczby wieksze od 10 i mniejsze od 100:\n");
	for (int i = 0; i < liczba_elementow; i++) {
		if (tablica[i] > 10 && tablica[i] < 100)
			printf("%d\n", tablica[i]);
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

	drukuj(liczby, i);

	return 0;
}
