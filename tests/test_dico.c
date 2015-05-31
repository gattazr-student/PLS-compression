#include <dictionnaire.h>
#include <stdlib.h>
#include <stdio.h>


int main(){

	Code a, b, c, d, e, f, g, h, i;
	char * s = NULL;
	int wI;
	int lg, lg1;

	initialiser();
	a = inserer(82,69);
	c = existe_code(259);
	d = inserer(259,65);
	e = existe_code(260);
	f = existe_seq(259,65, &b);
	g = inserer(260,84);
	h = inserer(261,72);
	i = inserer(262,65);
	s = codeVersSequence(263, &lg);

	printf("inserer(67,79) : %d\n", a);
	printf("existe_code(259) : %d\n", c);
	printf("inserer(259,85) : %d\n", d);
	printf("existe_code(260) : %d\n", e);
	printf("existe_seq(259,65,&b) : %d\n", f);
	printf("inserer(260,84) : %d\n", g);
	printf("inserer(261,72) : %d\n", h);
	printf("inserer(262,65) : %d\n", i);
	printf("codeVersSequence(263, &lg) : %s\n", s);
	printf("longueur de la séquence : %d\n", lg);

	/* longue séquence pour tester codeVersSequence */
	b = 'B';
	a = 'A';
	for(wI = 0; wI < 64; wI++){
		a = inserer(a, b);
		b++;
	}
	s = codeVersSequence(a, &lg1);
	printf("codeVersSequence(%d, &lg1) : %s\n", a, s);
	printf("longueur de la séquence : %d\n", lg1);

	/* Très longue séquence séquence */
	a = 0;
	for(wI = 0; wI < 1000; wI++){
		a = inserer(a, b);
		b++;
	}

	return 0;
}
