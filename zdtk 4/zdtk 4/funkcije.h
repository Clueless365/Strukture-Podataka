#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FUNKCIJE_H
#define FUNKCIJE_H

struct poly;
typedef struct _poly* pos;
typedef struct _poly
{
	int coeff;
	int expo;
	pos next;
}poly;

int OpenFileAndCallFetch(pos head1, pos head2);
int FetchData(FILE* fp, pos head1, pos head2);
pos CreateElem();
int SortedInsertionDescending(pos head, int coeff, int expo);
int ReverseList();
int AddPolynomial(pos head1, pos head2, pos HeadResult);
int MulitplyPolynomial(pos head1, pos head2, pos HeadResult);
int Menu(pos head1, pos head2, pos HeadAdditionResult, pos HeadMultiplicationResult);
int Display(pos head1, pos head2, pos head3, pos head4);
int DeleteAll(pos head1, pos head2, pos head3, pos head4);

#endif