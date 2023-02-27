#include <stdio.h>
#include <stdlib.h>
#define NMAX 10
#define NMAX2 100

void frequency(int n, int v[NMAX])  // formare vector de frecventa
{
	while (n) {
		v[n % 10]++;
		n /= 10;
	}
}

int increasing(int v[NMAX])  // formare numar in ordine crescatoare
{
	int i, cres = 0;
	for (i = 1; i < 10; i++) {
		while (v[i] != 0) {
			cres = cres * 10 + i;
			v[i]--;
		}
	}
	return cres;
}

int decreasing(int v[NMAX])  // formare numar in ordine descrescatoare
{
	int i, des = 0, p = 1;
	for (i = 0; i < 10; i++) {
		while (v[i] != 0) {
			des = des + p * i;
			v[i]--;
			p *= 10;
		}
	}
	return des;
}

int subtraction(int cres, int des)  // diferenta numerelor
{
	return des - cres;
}

void periodicity(int n)
{
	int v[NMAX] = {0}, i, j, cres, des, w[NMAX2], m = 0, ok = 1, nr, s = 0, i2;
	while (ok == 1) {
		frequency(n, v);
		cres = increasing(v);
		for (i = 0; i < 10; i++)
			v[i] = 0;
		frequency(n, v);
		des = decreasing(v);
		w[m++] = subtraction(cres, des);  // se salveaza in vector diferentele
		if (m == 0)
			continue;
		nr = m - 1;
		for (i = 0; i < m - 1 && ok == 1; i++) {
			if (w[i] == w[nr]) {
				ok = 0;  // se verifica daca exista in vector acea diferenta
				for (j = 0; j < i; j++)
					s++;
				for (i2 = i; i2 < m - 1; i2++)
					if (w[i2] == w[nr])
						break;
				printf("%u\n", s);
				for (j = i; j < nr; j++)
					printf("%u ", w[j]);
				break;
			}
		}
		n = w[m - 1];
		for (i = 0; i < 10;
				 i++)  // initializare vector de frecventa cu 0 urmatorul numar
			v[i] = 0;
	}
}

int main(void)
{
	int n;
	scanf("%u", &n);
	periodicity(n);
	return 0;
}
