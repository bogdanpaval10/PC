#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 100
#define NMAX2 11
#define NMAX3 20

typedef struct{
	char p[3];
	int w, h, max;
	int **mat;
} picture;

typedef struct{
	int x1, x2, y1, y2;
} coord;

int **allocmat(int n, int m) // alocare matrice
{
	int **mat, i;
	mat = (int **)malloc(n * sizeof(int *));
	if (!mat)
		exit(1);
	for (i = 0; i < n; i++) {
		mat[i] = (int *)malloc(m * sizeof(int));
		if (!mat[i])
			exit(1);
	}
	return mat;
}

void dezalloc(int n, int **mat) // dezalocare matrice
{
	int i;
	for (i = 0; i < n; i++)
		free(mat[i]);
	free(mat);
}

int load(char namef[20], int *h, int *w, picture *pic, int *pow, int *maxi, int *ok_loaded)
{
	int i, j;
	FILE *fin = fopen(namef, "r");
	if (!fin) {
		printf("Failed to load %s\n", namef);
		return 0;
	}
	fscanf(fin, "%s", pic->p); // citire date din fisier
	if ((strcmp(pic->p, "P2") == 0) || (strcmp(pic->p, "P5") == 0)) {
		fscanf(fin, "%d %d %d\n", &pic->w, &pic->h, &pic->max); // daca e gray
		*pow = 1;
	}
	if ((strcmp(pic->p, "P3") == 0) || (strcmp(pic->p, "P6") == 0)) {
		fscanf(fin, "%d %d %d\n", &pic->w, &pic->h, &pic->max); // daca e color
		*pow = 3;
	}
	*h = pic->h;
	*w = *pow * pic->w;
	if(pic->h >= pic->w * (*pow))
		*maxi = pic->h;
	else
		*maxi = pic->w * (*pow);
	
	pic->mat = allocmat(*maxi, *maxi);
	if (!pic->mat) {
		printf("Failed to load %s\n", namef);
		return 0;
	}
	if ((strcmp(pic->p, "P2") == 0) || (strcmp(pic->p, "P3") == 0))
		for (i = 0; i < pic->h; i++) // citire matrice daca este ascii
			for (j = 0; j < *pow * pic->w; j++)
				fscanf(fin, "%d", &pic->mat[i][j]);
	if ((strcmp(pic->p, "P5") == 0) || (strcmp(pic->p, "P6") == 0))
		for (i = 0; i < pic->h; i++) // citire matrice daca este binar
			for (j = 0; j < *pow * pic->w; j++)
				fread(&pic->mat[i][j], sizeof(char), 1, fin);
	fclose(fin);
	printf("Loaded %s\n", namef);
	*ok_loaded = 1;
	return 1;
}

void selectall(picture pic, coord *sel) // selecteaza toata imaginea
{
	sel->x1 = 0;
	sel->y1 = 0;
	sel->x2 = pic.w;
	sel->y2 = pic.h;
}

int selectt(int x1, int y1, int x2, int y2, coord *sel, picture pic, int ok_ld)
{ // selecteaza o anumita zona
	if (ok_ld != 1) { // daca au fost citite coordonate valide
		printf("No image loaded\n");
		return 0;
	}
	if (x1 == x2 && y1 == y2 && x1 == y1) {
		printf("Invalid set of coordinates\n");
		return 0;
	}
	if (x1 < 0 || y1 < 0 || x2 > pic.w || y2 > pic.h || x1 == x2) {
		printf("Invalid set of coordinates\n");
		return 0;
	}
	if (x1 > pic.w || y1 > pic.h || x2 < 0 || y2 < 0 || y1 == y2) {
		printf("Invalid set of coordinates\n");
		return 0;
	}
	if (x1 > x2) {
		sel->x1 = x2;
		sel->x2 = x1;
	} else {
		sel->x1 = x1;
		sel->x2 = x2;
	}
	if (y1 > y2) {
		sel->y1 = y2;
		sel->y2 = y1;
	} else {
		sel->y1 = y1;
		sel->y2 = y2;
	}
	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
	return 1;
}

int rotate(picture *pic, coord *sel, int pow, int angle)
{
	int n, m, maxi = NMAX2, sign = 1, nr, verif_all = 0, i, j, k, aux;
	int **cpy1, **cpy2;
	if (angle % 90 != 0) {  // verifica daca datele introduse sunt valide
		printf("Unsupported rotation angle\n");
		return 0;
	}
	n = sel->y2 - sel->y1;
	m = sel->x2 - sel->x1;
	if (!pic->mat)
		return 0;
	if (sel->x2 == pic->w && sel->y2 == pic->h)
		if (sel->x1 == 0 && sel->y1 == 0) // identifica tipul selectiei
			verif_all = 1;
	if (verif_all == 0)
		if ((sel->x2 - sel->x1) != (sel->y2 - sel->y1)) {
			printf("The selection must be square\n");
			return 0;
		}
	if (angle < 0)
		sign = -1;
	angle = angle * sign;
	nr = angle / 90;
	nr = nr % 4;
	if (sign == -1)
		nr = 4 - nr;
	if(pic->w > pic->h)
		maxi += pic->w;
	else
		maxi += pic->h;
	cpy1 = allocmat(maxi, maxi * pow); // aloca memorie si copiaza selectia
	cpy2= allocmat(maxi, maxi * pow);
	for (i = sel->y1; i < sel->y2; i++)
		for (j = pow * sel->x1; j < pow * sel->x2; j++)
			cpy2[i - sel->y1][j - pow * sel->x1] = pic->mat[i][j];
	while (nr != 0) {
		for (i = 0; i < n; i++)
			for (j = 0; j < pow * m; j = j + pow)
				for (k = 0; k < pow; k++)
					cpy1[j / pow][pow * (n - i - 1) + k] =
						cpy2[i][j + k];
		if (verif_all == 1) {
			aux = n;
			n = m;
			m = aux;
		}
		for (i = 0; i < n; i++)
			for (j = 0; j <  pow* m; j++)
				cpy2[i][j] = cpy1[i][j];
		nr--;
	}
	if (verif_all == 1 && pic->h != n) { // interschimba dimensiuni si selectia
		aux = pic->h;
		pic->h = pic->w;
		pic->w = aux;
		aux = sel->y2;
		sel->y2 = sel->x2;
		sel->x2 = aux;
	} // copiaza in matricea initiala imaginea rotita
	for (i = sel->y1; i < sel->y2; i++)
		for (j = pow * sel->x1; j < pow * sel->x2; j++)
			pic->mat[i][j] = cpy2[i - sel->y1][j - pow * sel->x1];
	dezalloc(maxi, cpy1); // dezaloca matricele copii
	dezalloc(maxi, cpy2);
	printf("Rotated %d\n", sign * angle);
	return 1;
}

int crop(coord sel, int *h, int *w, picture *pic, int pow, int maxi, int ok_ld)
{
	if (ok_ld != 1) {
		return 0;
		printf("No image loaded\n");
	}		
	int i, j, **copy;
	pic->w = *w = sel.x2 - sel.x1; // inlocuieste noile dimensiuni
	pic->h = *h = sel.y2 - sel.y1;
	copy = allocmat(maxi, maxi); // aloca memorie si copiaza selectia
	for (i = 0; i < *h; i++)
		for (j = 0; j < pow * (*w); j++)
			copy[i][j] = pic->mat[sel.y1 + i][sel.x1 * pow + j];
	dezalloc(maxi, pic->mat); // dezaloca vechea matrice
	pic->mat = allocmat(maxi, maxi); // aloca cu noile dimensiuni
	for (i = 0; i < *h; i++) // copiaza imaginea micsorata
		for (j = 0; j < pow * (*w); j++)
			pic->mat[i][j] = copy[i][j];
	dezalloc(maxi, copy);
	printf("Image cropped\n");
	return 1;
}

int grayscale(picture *pic, coord sel, int pow, int ok_loaded)
{
	if (ok_loaded != 1) {
		printf("No image loaded\n");
		return 0;
	}	
	double newcol = 0;
	int i, j;
	if (pow == 3) {
		for (i = sel.y1; i < sel.y2; i++) // calzuleaza noile valori
			for (j = pow * sel.x1; j < pow * sel.x2; j += pow) {
				newcol = (pic->mat[i][j] + pic->mat[i][j + 1] + 
				pic->mat[i][j + 2]) / 3;
				if (newcol - (int)newcol >= 0.5)
					newcol++;
				pic->mat[i][j] = newcol; // inlocuieste cu noile valori
				pic->mat[i][j + 1] = newcol;
				pic->mat[i][j + 2] = newcol;
			}
		printf("Grayscale filter applied\n");
	} else {
		printf("Grayscale filter not available\n");
	}
	return 1;
}

int sepia(picture *pic, coord sel, int pow, int ok_loaded)
{
	if (ok_loaded != 1) {
		printf("No image loaded\n");
		return 0;
	}			
	double newred, newgreen, newblue;
	int i, j;
	if (pow == 3) {
		for (i = sel.y1; i < sel.y2; i++) // calculeaza noile valori
			for (j = sel.x1 * pow; j < pow * sel.x2; j += pow) {
				newred = 0.393 * pic->mat[i][j] + 0.769 * pic->mat[i][j + 1] +
				 0.189 * pic->mat[i][j + 2];
				if (newred > pic->max)
					newred = pic->max;
				if (newred - (int)newred >= 0.5)
					newred++;
				newgreen = 0.349 * pic->mat[i][j] + 0.686 * pic->mat[i][j + 1]
				 + 0.168 * pic->mat[i][j + 2];
				if (newgreen > pic->max)
					newgreen = pic->max;
				if (newgreen - (int)newgreen >= 0.5)
					newgreen++;
				newblue = 0.272 * pic->mat[i][j] + 0.534 * pic->mat[i][j + 1] +
				 0.131 * pic->mat[i][j + 2];
				if (newblue > pic->max)
					newblue = pic->max;
				if (newblue - (int)newblue >= 0.5)
					newblue++;
				pic->mat[i][j] = (int)newred; // inlocuieste cu noile valori
				pic->mat[i][j + 1] = (int)newgreen;
				pic->mat[i][j + 2] = (int)newblue;
			}
		printf("Sepia filter applied\n");
	} else {
		printf("Sepia filter not available\n");
	}
	return 1;
}

int save(char namef[20], picture pic, int ok, int pow, int verif_load)
{
	if (verif_load != 1) {
		printf("No image loaded\n");
		return 0;
	}				
	int i, j;
	if (ok == 1) { // daca fisierul trebuie sa fie text
		FILE *fout = fopen(namef, "w");
		if (!fout) // deschide si afiseaza primele date in fisier
			exit(1);
		fprintf(fout, "%s\n", pic.p);
		fprintf(fout, "%d %d\n", pic.w, pic.h);
		fprintf(fout, "%d\n", pic.max);
		for (i = 0; i < pic.h; i++) { // afiseaza pixelii imaginii
			for (j = 0; j < pow * pic.w; j++)
				fprintf(fout, "%d ", (unsigned char)pic.mat[i][j]);
			fprintf(fout, "\n");
			}
		fclose(fout);
		printf("Saved %s\n", namef);
	} else { // daca fisierul trebuie sa fie binar
		FILE *fout = fopen(namef, "wb");
		if (!fout) // deschide si afiseaza primele date in fisier
			exit(1);
		fprintf(fout, "%s\n", pic.p);
		fprintf(fout, "%d %d\n", pic.w, pic.h);
		fprintf(fout, "%d\n", pic.max);
		for (i = 0; i < pic.h; i++) { // afiseaza pixelii imaginii
			for (j = 0; j < pow * pic.w; j++)
				fwrite(&pic.mat[i][j], sizeof(char), 1, fout);
		}
		fclose(fout);
		printf("Saved %s\n", namef);
	}
	return 1;
}

void exitt(picture pic, int maxi, int pow, int verif_load, int *ok)
{
	if (pow != 0 && verif_load == 1)
		dezalloc(maxi, pic.mat); // dezaloca matricea
	*ok = 1;
}

void read(void)
{
	char command[NMAX2], namef[NMAX3], line[NMAX], *p, cline[NMAX], word[NMAX2];
	coord sel;
	picture pic;
	int angle, h, w, pow = 0, verif_load, maxi, ok_loaded = 0, verif_save;
	while (1) {
		int ok = 0, x1, x2, y1, y2;
		fgets(line, NMAX, stdin); // citeste linia comenzii si ii face o copie
		strcpy(cline, line);
		p = strtok(line, " \n");
		strcpy(command, p);
		if (strcmp(command, "LOAD") == 0) { //  daca este comanda load
			if (pow != 0) // dezaloca memoria daca a mai fost un load inainte
				dezalloc(maxi, pic.mat);
			p = strtok(NULL," \n");
			strcpy(namef, p); // parseaza numele fisierului de citire
			verif_load = load(namef, &h, &w, &pic, &pow, &maxi, &ok_loaded);
			selectall(pic, &sel);
			ok = 1;
		}
		if (strcmp(command, "SELECT") == 0) { // daca este comanda select
			if (sscanf(cline, "%s %d %d %d %d", command, &x1, &y1, &x2, &y2) 
			== 5)
				selectt(x1, y1, x2, y2, &sel, pic, ok_loaded);
			else {
				p = strtok(NULL," \n");
				if (strcmp(p, "ALL") == 0) { // daca este select all
					if (ok_loaded == 1)
						selectall(pic, &sel);
					else
						printf("No image loaded\n");
					if (pow != 0)
						printf("Selected ALL\n");
				} else
					printf("Invalid command\n");
			}
			ok = 1;
		}
		if (strcmp(command, "ROTATE") == 0) { // daca este comanda rotate
			if (sscanf(cline, "%s %d", command, &angle) == 2) { //citeste unghi
				if (ok_loaded == 1)
					rotate(&pic, &sel, pow, angle);
				else
					printf("No image loaded\n");
				ok = 1;
			}
		}
		if (strcmp(command, "CROP") == 0) { // daca este comanda crop
			if (ok_loaded == 1) {
				crop(sel, &h, &w, &pic, pow, maxi, ok_loaded);
				selectall(pic, &sel);
			}
			ok =1;
		}
		if (strcmp(command, "GRAYSCALE") == 0) { // daca este comanda grayscale
			grayscale(&pic, sel, pow, ok_loaded);
			ok = 1;
		}
		if (strcmp(command, "SEPIA") == 0) { // daca este comanda sepia
			sepia(&pic, sel, pow, ok_loaded);
			ok = 1;
		}
		if (strcmp(command, "SAVE") == 0) { // daca este comanda save
			if (sscanf(cline, "%s %s %s", command, namef, word) == 3)
				if (strcmp(word, "ascii") == 0) { // daca fisierul e text
					if (strcmp(pic.p, "P5") == 0) // daca e cazul,schimba tipul
						strcpy(pic.p, "P2");
					if (strcmp(pic.p, "P6") == 0)
						strcpy(pic.p, "P3");
					verif_save = 1;
				} else
					printf("Invalid command\n");
			else // citeste argumentele
				if (sscanf(cline, "%s %s", command, namef) == 2) {
					if (strcmp(pic.p, "P2") == 0) // daca fisierul e binar
						strcpy(pic.p, "P5");
					if (strcmp(pic.p, "P3") == 0) // daca e cazul,schimba tipul
						strcpy(pic.p, "P6");
					verif_save = 0;
				}
			if ((verif_save == 1) || (verif_save == 0))
					save(namef, pic, verif_save, pow, verif_load);
			ok = 1;
		}
		if (strcmp(command, "EXIT") == 0) { // daca este comanda exit
			exitt(pic, maxi, pow, verif_load, &ok);
			break; // opreste programul
		}
		if (ok == 0) // comanda este invalida
			printf("Invalid command\n");
	}
}

int main(void)
{
	read();
	return 0;
}
