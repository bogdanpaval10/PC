#include <stdio.h>
#define NMAX1 100
#define NMAX2 10000

void mat_array(int a[NMAX1][NMAX1], int n, int m)
{
	int i, j;
	for (i = 0; i < n; i++) { // memorare matrice
		for (j = 0; j < m; j++)
			scanf("%d", &a[i][j]);
	}
}

int v_array(int v[NMAX2], int n)
{
	int i, j, nv = 0;
	for (i = 0; i < n; i++) {  // memorare restrictii pentru linii
		scanf("%d", &j);
		v[nv++] = j;
		while (j != 0) {
			scanf("%d", &v[nv++]);
			j--;
		}
	}
	return nv;
}

int w_array(int w[NMAX2], int m)
{
	int i, j, nw = 0;
	for (i = 0; i < m; i++) {  // memorare restrictii pentru coloane
		scanf("%d", &j);
		w[nw++] = j;
		while (j != 0) {
			scanf("%d", &w[nw++]);
			j--;
		}
	}
	return nw;
}

int check(void)
{
	int n, m, i, j, a[NMAX1][NMAX1], v[NMAX2], w[NMAX2], nw, nv;
	scanf("%d", &n);
	scanf("%d", &m);
	nv = v_array(v, n);
	nw = w_array(w, m);
	mat_array(a, n, m);
	int avs, copy, unde = 0, ok;
	for (i = 0; i < n; i++) {  // verificarea restrictiilor de pe linii
		copy = unde++;
		j = 0;
		while (j < m) {
			avs = 0;
			ok = 1;
			while (a[i][j] == 0 && j < m)
				j++;
			while (a[i][j] == 1 && j < m) {
				avs++;
				j++;
				ok = 0;
			}
			if (avs == v[unde] && unde < nv && avs != 0) {
				v[copy]--;
				v[unde] -= avs;
				unde++;
			}
			if (ok == 0)
				j--;
			j++;
		}
		if (unde > nv && avs > 0)
			v[copy] = -1;
	}
	for (i = 0; i < nv; i++) {
		if (v[i] != 0)
			return 0;
	}
	unde = 0;
	for (j = 0; j < m; j++) {  // verificarea restrictiilor de pe coloane
		copy = unde++;
		i = 0;
		while (i < n) {
			avs = 0;
			ok = 1;
			while (a[i][j] == 0 && i < n)
				i++;
			while (a[i][j] == 1 && i < n) {
				avs++;
				i++;
				ok = 0;
			}
			if (avs == w[unde] && unde < nw && avs != 0) {
				w[copy]--;
				w[unde] -= avs;
				unde++;
			}
			if (ok == 0)
				i--;
			i++;
		}
		if (unde > nw && avs > 0)
			w[copy] = -1;
	}
	for (i = 0; i < nw; i++) {
		if (w[i] != 0)
			return 0;
	}
	return 1;
}

int main(void)
{
	int t;
	scanf("%d", &t);
	while (t != 0) {
		if (check() == 1)
			printf("Corect\n");
		else
			printf("Eroare\n");
		t--;
	}
	return 0;
}
