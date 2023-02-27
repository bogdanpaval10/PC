#include <stdio.h>
#define NMAX1 2
#define NMAX2 3
int b_form(int n, int a[NMAX1][n], int b[NMAX2][n])  // formare matricea b
{
	int i, nr = 0;
	for (i = 0; i < n; i++) {
		if (a[0][i] != 10) {
			b[0][nr] = a[0][i];                   // nota
			b[1][nr] = a[1][i];                   // puncte
			b[2][nr] = a[1][i] * (10 - a[0][i]);  // posibilitate marire punctaj
			nr++;
		}
	}
	return nr;
}

void b_sort(int nr, int n, int b[NMAX2][n])
{
	int i, j, m; //sortare matricea b in functie de ultima linie
	for (i = 0; i < nr - 1; i++) {
		for (j = i + 1; j < nr; j++) {
			if (b[2][i] < b[2][j]) {
				m = b[0][i];
				b[0][i] = b[0][j];
				b[0][j] = m;
				m = b[1][i];
				b[1][i] = b[1][j];
				b[1][j] = m;
				m = b[2][i];
				b[2][i] = b[2][j];
				b[2][j] = m;
			}
		}
	}
}

int count(int n, int a[NMAX1][n], int b[NMAX2][n], int p_min)
{
	int i, j, s = 0, m;  // calculare punctaj si aflare numarul minim de mariri
	for (i = 0; i < n; i++)
		s += a[0][i] * a[1][i];
	if (s >= p_min) {
		m = -1;
	} else {
		j = 0;
		while (s < p_min) {
			s += b[2][j];
			j++;
		}
		m = j;
	}
	return m;
}

int main(void)
{
	int n, p_min, i, j, nr, m, ok = 1;
	scanf("%d", &n);
	int a[NMAX1][n], b[NMAX2][n];
	for (i = 0; i < 2; i++) {
		for (j = 0; j < n; j++) {
			scanf("%d", &a[i][j]);
			if (a[0][j] != 10)
				ok = 0;
		}
	}
	nr = b_form(n, a, b);
	scanf("%d", &p_min);
	b_sort(nr, n, b);
	m = count(n, a, b, p_min);
	if (ok == 1)
		m = -1;
	printf("%d\n", m);
	return 0;
}
