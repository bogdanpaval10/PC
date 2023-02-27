
#include <stdio.h>
#include <stdlib.h>

int *alloc_vect(int n) // alocare vector cu nr de coloane
{
	int *nrcol;
	nrcol = (int *)malloc(n * sizeof(int));
	return nrcol;
}

char **alloc_read(int n, int *nrcol) // alocare si citire matrice
{
	int i, j, k, where;
	unsigned int x;
	char **mat;
	mat = (char **)malloc(n * sizeof(char *));
	if (mat == 0)
		exit(1);

	for (i = 0; i < n; i++) {
		scanf("%d", &nrcol[i]);
		mat[i] = (char *)malloc(nrcol[i] * 4 * sizeof(char));
		nrcol[i] = nrcol[i] * 4 * sizeof(char);
		if (mat[i] == 0)
			exit(1);
		where = 0;
		for (j = 0; j < nrcol[i] / 4; j++) {
			scanf("%X", &x);
			for (k = 0; k < 4; k++) {
				mat[i][where++] = x % 256;
				x /= 256;
			}
		}
	}
	return mat;
}

void dezalloc(int n, char **mat, int *nrcol) // dezalocare vector si matrice
{
	for (int i = 0; i < n; i++)
		free(mat[i]);
	free(mat);
	free(nrcol);
}

double average(char **mat, int *nrcol, int n)
{
	int i, j, nr = 0, sum = 0;
	for (j = 0; j < nrcol[0] - 1; j++) { // suma de pe prima linie
		sum += mat[0][j];
		nr++;
	}
	for (j = 0; j < nrcol[n - 1] - 1 && n != 1; j++) {
		sum += mat[n - 1][j]; // suma de pe ultima linie
		nr++;
	}
	for (i = 1; i < n - 1; i++) { // suma de pe prima coloana
		sum += mat[i][0];
		nr++;
	}
	for (i = 0; i < n; i++) { // suma de pe ultima coloana
		sum += mat[i][nrcol[i] - 1];
		nr++;
	}
	return (double)sum / nr;
}

void modify(char **mat, int *nrcol, int l, int ind, char dim, unsigned int val)
{
	int j, nr;
	if (dim == 'C') { // realocare matrice daca elem este in afara zonei
		if (ind > nrcol[l]) {
			if (ind % 4 != 0)
				nr = ind / 4 + 1;
			else
				nr = ind / 4;
			mat[l] = (char *)realloc(mat[l], nr * sizeof(char));
			for (j = nrcol[l]; j < 4 * nr; j++)
				mat[l][j] = 0 % 256;
			nrcol[l] = 4 * nr;
		}
		mat[l][ind - 1] = val % 256; // inlocuire element cu noua valoare
	}

	if (dim == 'S') { // realocare matrice daca elem este in afara zonei
		if (ind * 2 - 1 > nrcol[l]) {
			if (ind % 2 != 0)
				nr = ind / 2 + 1;
			else
				nr = ind / 2;
			mat[l] = (char *)realloc(mat[l], nr * 4 * sizeof(char));
			for (j = nrcol[l]; j < 4 * nr; j++)
				mat[l][j] = 0 % 256;
			nrcol[l] = 4 * nr;
		}
		mat[l][2 * ind - 2] = val % 256; // inlocuire elemente cu noile valori
		val = val / 256;
		mat[l][2 * ind - 1] = val % 256;
	}

	if (dim == 'I') { // realocare matrice daca elem este in afara zonei
		if (ind * 4 - 3 > nrcol[l]) {
			mat[l] = (char *)realloc(mat[l], ind * 4 * sizeof(char));
			for (j = nrcol[l]; j < 4 * ind; j++)
				mat[l][j] = 0 % 256;
			nrcol[l] = ind * 4;
		}
		for (j = 0; j < 4; j++) { // inlocuire elemente cu noile valori
			mat[l][ind * 4 - 4 + j] = val % 256;
				val /= 256;
		}
	} 
}

void del(char **mat, int l, int ind, char dim) // inlocuire elemente cu 0
{
	if (dim == 'C')
		mat[l][ind - 1] = 0;

	if (dim == 'S') {
		mat[l][2 * ind - 2] = 0;
		mat[l][2 * ind - 1] = 0;
	}

	if (dim == 'I') {
		mat[l][4 * ind - 4] = 0;
		mat[l][4 * ind - 3] = 0;
		mat[l][4 * ind - 2] = 0;
		mat[l][4 * ind - 1] = 0;
	}
}

void print_mat(int n, char **mat, int *nrcol)
{
	int i, j, p, k;
	unsigned int nr;
	for (i = 0; i < n; i++) {
		for (j = 0; j < nrcol[i]; j += 4) { // afisare matrice in hexazecimal
			p = 1;
			nr = 0;
			for (k = 0; k < 4; k++) {
				nr += (unsigned char)mat[i][j + k] * p;
				p *= 256;
			}
			printf("%08X ", (unsigned int)nr);
		}
		printf("\n");
	}
}

void swap(char **mat, int l, int ind, int dim)
{
	int a, b;
	if (dim == 'S') { // interschimbare 2 elemente
		a = mat[l][2 * ind];
		mat[l][2 * ind] = mat[l][2 * ind + 1];
		mat[l][2 * ind + 1] = a;
	}

	if (dim == 'I') { // interschimbare 4 elemente
		a = mat[l][4 * ind];
		mat[l][4 * ind] = mat[l][4 * ind + 3];
		b = mat[l][4 * ind + 1];
		mat[l][4 * ind + 1] = mat[l][4 * ind + 2];
		mat[l][4 * ind + 3] = a;
		mat[l][4 * ind + 2] = b;
	}
}

void task2(char **mat, int *nrcol)
{
	char op, dim;
	int line, index;
	unsigned int val;
	scanf("%c", &op);
	scanf("%c", &op);
	scanf("%c", &dim);
	scanf("%c", &dim);
	scanf("%d", &line);
	scanf("%d", &index);

	if (op == 'M') { // apelare functii in functie de operator
		scanf("%X", &val);
		modify(mat, nrcol, line, index, dim, val);
	}

	if (op == 'D')
		del(mat, line, index, dim);

	if (op == 'S')
		swap(mat, line, index, dim);
}

int sum(int n, char **mat, int *nrcol, int i, int j)
{
	int s = 0;
	if (mat[i][j] == 0) { // verificare element 0 si calculare suma
		s++;
		mat[i][j] = 1;
		if ((i - 1 >= 0) && nrcol[i - 1] > j)
			s += sum(n, mat, nrcol, i - 1, j);
		if ((i + 1 < n) && nrcol[i + 1] > j)
			s += sum(n, mat, nrcol, i + 1, j);
		if (j - 1 >= 0)
			s += sum(n, mat, nrcol, i, j - 1);
		if (j + 1 < nrcol[i])
			s += sum(n, mat, nrcol, i, j + 1);
	}

	return s;
}

void task3(int n, char **mat, int *nrcol, int *cpy_i, int *cpy_j, int *smax)
{
	int i, j, s;
	for (i = 0; i < n; i++) // verificare pentru fiecare element
		for (j = 0; j < nrcol[i]; j++) {
			s = sum(n, mat, nrcol, i, j);
			if (*smax < s) { // memorare pozitii si suma maxima
				*cpy_i = i;
				*cpy_j = j;
				*smax = s;
			}
		}
}

int main(void)
{
	int n, *nrcol, i, k, smax = 0, copy_i, copy_j;
	char **mat;
	double avrge = 0;
	scanf("%d", &n);
	nrcol = alloc_vect(n);
	mat = alloc_read(n, nrcol);
	avrge = average(mat, nrcol, n);

	scanf("%d", &k);
	for (i = 0; i < k; i++)
		task2(mat, nrcol);

	printf("task 1\n");
	printf("%.8lf\n", avrge);

	printf("task 2\n");
	print_mat(n, mat, nrcol);

	task3(n, mat, nrcol, &copy_i, &copy_j, &smax);
	printf("task 3\n");
	printf("%d %d %d", copy_i, copy_j, smax);
	dezalloc(n, mat, nrcol);
	return 0;
}
