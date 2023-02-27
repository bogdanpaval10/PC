#include <stdio.h>

int main(void)
{
	int n, x0, x1, x2, xmax_impar = -1, xmin_par = 1000000, i;
	long s = 0;
	double m_a = 0, nr = 0;
	scanf("%u", &n);
	if (n < 3) {  // daca n < 3 nu pot exista copaci speciali
		xmin_par = 0;
		xmax_impar = 0;
	} else {
		scanf("%u", &x0);
		scanf("%u", &x1);
		for (i = 2; i < n; i++) {
			scanf("%u", &x2);
			if (x1 > x0 && x1 > x2) {  // se verifica daca x1 este copac special
				s = s + x1;
				nr++;
				if ((i - 1) % 2 == 0 && xmin_par > x1)
					xmin_par = x1;
				if ((i - 1) % 2 == 1 && xmax_impar < x1)
					xmax_impar = x1;
			}
			x0 = x1;
			x1 = x2;
		}
		if (s == 0 && nr == 0) {  // daca nu exista niciun copac in padure
			xmax_impar = 0;
			xmin_par = 0;
		} else {
			m_a = s / nr;
	}
		if (xmin_par == 1000000) {  // daca nu exista niciun copac special cu
			xmin_par = -1;  // inaltimea minima pe pozitie para
		}
	}
	printf("%lu\n", s);
	printf("%.7f\n", m_a);
	printf("%d\n", xmax_impar);
	printf("%d\n", xmin_par);
	return 0;
}
