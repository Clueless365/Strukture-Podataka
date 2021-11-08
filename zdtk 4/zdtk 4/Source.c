#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funkcije.h"

/*Napisati program za zbrajanje i množenje polinoma. Koeficijenti i eksponenti se
čitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani.*/

int main()
{
	poly head1 = { .coeff = 0, .expo = 0, .next = NULL };
	poly head2 = { .coeff = 0, .expo = 0, .next = NULL };
	poly HeadMultiplicationResult = { .coeff = 0, .expo = 0, .next = NULL };
	poly HeadAdditionResult = { .coeff = 0, .expo = 0, .next = NULL };

	pos p1 = &head1;
	pos p2 = &head2;
	pos p3 = &HeadAdditionResult;
	pos p4 = &HeadMultiplicationResult;
	
	OpenFileAndCallFetch(p1, p2);
	AddPolynomial(p1, p2, p3);
	MulitplyPolynomial(p1, p2, p4);
	Menu(p1, p2, p3, p4);
	Display(p1, p2, p3, p4);
	
	DeleteAll(p1, p2, p3, p4);
	
	return EXIT_SUCCESS;
}

