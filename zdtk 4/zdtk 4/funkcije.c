#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funkcije.h"

#define MAX 128
#define MAXLINE 1024

pos CreateElem()
{
	pos NewEl = NULL;
	NewEl = (pos)malloc(sizeof(poly));

	if (!NewEl)
	{
		perror("malloc greska!");
		return EXIT_FAILURE;
	}
	return NewEl;
}

int FetchData(FILE* fp, pos head1, pos head2)
{
	char buffer[MAX] = { 0 };
	char* NewBuffer; //"dinamicki" string
	//char c1, c2 = 'a'; ovi su bili za provjeru trash-a
	int coeff = 0;
	int expo = 0;
	int i = 0;
	int n = 0;
	int counter = 0;
	int check = 0;

	while (!feof(fp)) //mogli bi i samo 2 iteracije jer je poznat br polinoma
	{
		fgets(buffer, MAX, fp);
		NewBuffer = buffer;

		while (strlen(NewBuffer) != 0)
		{
			check = sscanf(NewBuffer, " %d %d %n", &coeff, &expo, &n);
			if(check!= 2)
			{
				perror("Ne valja datoteka!\n");
				return EXIT_FAILURE;
			}
			//los nacin provjere trasha
			/*sscanf(NewBuffer, " %d%c%d%c%n", &coeff, &c1, &expo, &c2, &n);
			if (coeff == NULL || expo == NULL || c1 != ' ' || c2 != ' ')//nije 100% sig.
			{//provjera trash-a
				perror("Ne valja datoteka!");
				return EXIT_FAILURE;
			}*/

			if (counter == 0)
				SortedInsertionDescending(head1, coeff, expo);//sortirani upis 1. pol
			else
				SortedInsertionDescending(head2, coeff, expo);

			NewBuffer += n;//mice pocetak stringa do 1. inta
		}
		counter++;
	}

	return EXIT_SUCCESS;
}

int OpenFileAndCallFetch(pos head1, pos head2)//prototip
{
	char FileName[MAX] = { 0 };
	FILE* fp = NULL;


	printf("upisi ime datoteke: (default 'polinomi'): ");
	//scanf(" %s", FileName);
	fgets(FileName, MAX, stdin);
	FileName[strcspn(FileName, "\n")] = 0;//skida \n

	strcat(FileName, ".txt");

	fp = fopen(FileName, "r");
	if (fp == NULL)
	{
		perror("Datoteka nije otvorena");
		return EXIT_FAILURE;
	}

	FetchData(fp, head1, head2);

	fclose(fp);

	return EXIT_SUCCESS;
}

int SortedInsertionDescending(pos head, int coeff, int expo)
{
	pos temp = head;
	pos NewEl = NULL;

	while (temp->next != NULL && (temp->next->expo) > expo) //silazno
	{
		temp = temp->next;
	}

	if (temp->next != NULL && temp->next->expo == expo)//ako su eksponenti isti onda zbroji
	{
		temp->next->coeff += coeff;
		return EXIT_SUCCESS;
	}

	NewEl = CreateElem();
	NewEl->expo = expo;
	NewEl->coeff = coeff;
	NewEl->next = temp->next;
	temp->next = NewEl;

	return EXIT_SUCCESS;
}

int ReverseList(pos head)
{
	pos mid = head->next;
	pos i = NULL;
	pos j = NULL;

	while (mid != NULL)
	{
		j = mid->next;//spremimo sljedeci
		mid->next = i;//pointamo unatrag <-
		i = mid;//pomaknemo prvi u desno
		mid = j;//pomaknemo mid u desno
	}
	head->next = i; //stavljamo head iza zadnjeg odnosno sada je to 1.

	return EXIT_SUCCESS;
}

int AddPolynomial(pos head1, pos head2, pos HeadAdditionResult)
{
	pos temp1 = head1->next;
	pos temp2 = head2->next;
	pos temp3 = HeadAdditionResult;
	int zbroj = 0;

	while (temp1 != NULL || temp2 != NULL)// || da bi mogli zbrojit
	{									 //polnome razl. duljjine

		if (temp1->expo > temp2->expo)//ovo ne bi radilo bez ifa dolje
		{
			SortedInsertionDescending(temp3, temp1->coeff, temp1->expo);
			temp1 = temp1->next;//inkrementiramo samo ovaj

			if (temp1 == NULL)//treba ovo inace ne rade drugi if-ovi kada
			{				 //su liste razlicite duljine
				temp1 = CreateElem();
				temp1->coeff = 0;
				temp1->expo = 0;
				temp1->next = NULL;
			}
		}
		else if (temp2->expo > temp1->expo)
		{
			SortedInsertionDescending(temp3, temp2->coeff, temp2->expo);
			temp2 = temp2->next;

			if (temp2 == NULL)//treba ovo inace ne rade drugi if-ovi kada
			{				 //su liste razlicite duljine
				temp2 = CreateElem();
				temp2->coeff = 0;
				temp2->expo = 0;
				temp2->next = NULL;
			}
		}
		else //if (temp1->expo == temp2->expo)
		{
			zbroj = temp1->coeff + temp2->coeff;
			if (zbroj == 0)
				goto skip; //preskacemo upisivanje u rezultat
								   //ako zbroj koef  bude 0
			SortedInsertionDescending(temp3, zbroj, temp1->expo);
		skip:
			temp1 = temp1->next;
			temp2 = temp2->next;
		}
	}

	return EXIT_SUCCESS;
}

int MulitplyPolynomial(pos head1, pos head2, pos HeadMultiplicationResult)
{
	pos temp1 = head1->next;
	pos temp2 = head2->next;
	pos temp3 = HeadMultiplicationResult;


	while (temp1)//isto kao temp1 != NULL
	{
		while (temp2)
		{
			SortedInsertionDescending(temp3, temp1->coeff * temp2->coeff, temp1->expo + temp2->expo);
			temp2 = temp2->next;
		}
		temp2 = head2->next;//vracanje 2. pol na prvi element
		temp1 = temp1->next;//inkrement 1. pol
	}
	return EXIT_SUCCESS;
}

int Menu(pos head1, pos head2, pos HeadAdditionResult, pos HeadMultiplicationResult)
{
	int choice = 0;

	printf("Pritisnite\n-> 1 za silazno sortiranje (Oo.)\n"
		"-> 2 za uzlazno sortiranje (.oO) \n");
	scanf(" %d", &choice);

	switch (choice)
	{

	case 1:
		break;

	case 2: //za silazno sortiranje samo obrnemo listu
		ReverseList(head1);
		ReverseList(head2);
		ReverseList(HeadAdditionResult);
		ReverseList(HeadMultiplicationResult);
		break;

	default:
		printf("\nKrivi unos!");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int Display(pos head1, pos head2, pos head3, pos head4)//head 3 zbroj, 4 umnozak
{	//moze se jos uljepsat ispis kad se ispisuju negativni koef. i ^1 ekspo., ali 
	//tehnicki je tocno
	pos temp = head1->next;
	int i = 0;

	printf("\n1. Polinom: ");
	while (temp)
	{
		if (temp->next == NULL)
		{
			printf("%dX^%d\n", temp->coeff, temp->expo);
			temp = temp->next;
		}
		else
		{
			printf("%dX^%d + ", temp->coeff, temp->expo);
			temp = temp->next;
		}
	}

	temp = head2->next;

	printf("2. Polinom: ");
	while (temp)
	{
		if (temp->next == NULL)
		{
			printf("%dX^%d\n", temp->coeff, temp->expo);
			temp = temp->next;
		}
		else
		{
			printf("%dX^%d + ", temp->coeff, temp->expo);
			temp = temp->next;
		}
	}

	temp = head3->next;

	printf("\nZbroj: ");
	while (temp)
	{
		if (temp->next == NULL)
		{
			printf("%dX^%d\n", temp->coeff, temp->expo);
			temp = temp->next;
		}
		else
		{
			printf("%dX^%d + ", temp->coeff, temp->expo);
			temp = temp->next;
		}
	}

	temp = head4->next;

	printf("\nUmnozak: ");
	while (temp)
	{
		if (temp->next == NULL)
		{
			printf("%dX^%d\n\n", temp->coeff, temp->expo);
			temp = temp->next;
		}
		else
		{
			printf("%dX^%d + ", temp->coeff, temp->expo);
			temp = temp->next;
		}
	}

	return EXIT_SUCCESS;
}

int DeleteAll(pos head1, pos head2, pos head3, pos head4) //oslobodi mem.
{
	pos temp = head1->next;
	pos foll = NULL; //sljedeci (following)

	while (temp)
	{
		foll = temp->next;
		free(temp);
		temp = foll;
	}
	head1->next = NULL;

	temp = head2->next;
	foll = NULL;

	while (temp)
	{
		foll = temp->next;
		free(temp);
		temp = foll;
	}
	head2->next = NULL;

	temp = head3->next;
	foll = NULL;

	while (temp)
	{
		foll = temp->next;
		free(temp);
		temp = foll;
	}
	head3->next = NULL;

	temp = head4->next;
	foll = NULL;

	while (temp)
	{
		foll = temp->next;
		free(temp);
		temp = foll;
	}
	head4->next = NULL;

	return EXIT_SUCCESS;
}