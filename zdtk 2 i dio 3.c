#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50

struct _person;
typedef struct _person* position;
typedef struct _person {
	char name[MAX];
	char surname[MAX];
	int birthYear;
	position next;
}person;

position createPerson(char* name, char* surname, int birthYear);
int prependList(position head, char* name, char* surname, int birthYear);
int findLast(position head);
//int printlist(position first);
int printlist2(position head);
position findBySurname(position head, char* surname);
int delete(position head, char* surname);
int appendList(position head, char* name, char* surname, int birthYear);
int insertAfter(position ElemBefore, position newP); //insertati iskljucivo novi element
int insertBefore(position ElemAfter, position pos, position newP); //insertAfter je ucinkovitija


int main ()
{
	person head = {.next = NULL, .name = { 0 }, .surname = { 0 }, .birthYear = 0 };
	char name[MAX] = { 0 };
	char surname[MAX] = { 0 };
	int birthYear = 0;
	int n = 0;
	int i = 0;
	position novi; 

	position p = &head; //pointer na head element koji je cili inic. na 0 i NULL
	//privremeni unos
	printf("Koliko elemenata u listi?\n");
	scanf("%d", &n);
	
	for (i = 0; i < n; i++)
	{
		printf("Upisi ime, prezime i godinu\n");
		scanf(" %s", name);
		scanf(" %s", surname);
		scanf("%d", &birthYear);

		prependList(p, name, surname, birthYear);
	}
	//printlist(p->next);
	novi = createPerson("marko", "mrkovic", 2000);
	insertBefore(p, p->next, novi); //test funkcije
	printList2(p);
	
	return EXIT_SUCCESS;
}
position createPerson(char* name, char* surname, int birthYear)
{
	position newP = NULL;

	newP = (position)malloc(sizeof(person));
	if (newP == 0)
	{
		perror("malloc greska!");
		return -1;
	}
	strcpy(newP->name, name);
	strcpy(newP->surname, surname);
	newP->birthYear = birthYear;
	newP->next = NULL;

	return newP;
}

int prependList(position head, char* name, char* surname, int birthYear)
{//prepend - stavi na pocetak
	position newP = NULL; //pointer na novu osobu

	/*newP = (position)malloc(sizeof(person)); 

	if (!newP) //isto kao if(newp == NULL);
	{
		perror("malloc error!\n");
		return -1;
	}*/
	newP = createPerson(name, surname, birthYear); // ova funkcija minja zakomnetirane

	/*strcpy(newP->name, name);
	strcpy(newP->surname, surname);
	newP->birthYear = birthYear;
	newP->next = NULL;*/

	newP->next = head->next;
	head->next = newP;

	return EXIT_SUCCESS;
}

int findLast(position head)
{
	position temp = head;
	while (temp->next)
	{
		temp = temp->next;
	}
	return temp;
}

position findBySurname(position head, char* surname)
{
	position temp = head;
	while (temp->next)
	{
		if (strcmp(temp->next->surname, surname) == 0)
		{
			return temp->next;
		}
		temp = temp->next;
	}
	return NULL;
}

/*int printList(position first)
{
	position temp = first;
	while (temp) // moze i sa for petljom (citljivije)
	{
		printf("Name: %s, surname: %s, "
			"birth year: %d\n", temp->name, temp->surname, temp->birthYear);
		temp = temp->next;
	}
	return EXIT_SUCCESS;
}*/

int printList2(position head) //za vjezbu koristio head umisto first
{
	position temp = head;
	while (temp->next) // moze i sa for petljom (citljivije)
	{
		printf("Name: %s, surname: %s, "
			"birth year: %d\n", temp->next->name, temp->next->surname, temp->next->birthYear);
		temp = temp->next;
	}
	return EXIT_SUCCESS;
}

int delete (position head, char* surname)
{
	position toBeDeleted = findBySurname(head, surname);
	position temp = head;

	while (temp->next != toBeDeleted)
		temp = temp->next;

	temp->next = toBeDeleted->next;

	free(toBeDeleted);

	return EXIT_SUCCESS;
}

int appendList(position head, char* name, char* surname, int birthYear)
{//append - stavi na kraj
	position newP = NULL;
	position last = NULL;

	newP = createPerson(name, surname, birthYear);
	if (!newP) {
		perror("malloc greska!");
		return -1;
	}

	last = findLast(head);
	insertAfter(last, newP);

	return EXIT_SUCCESS;
}

int insertAfter(position ElemBefore, position newP)
{
	newP->next = ElemBefore->next;
	ElemBefore->next = newP;

	return EXIT_SUCCESS;
}

int insertBefore(position head, position ElemAfter, position newP)
{
	position temp = head;
	
	while (temp->next != ElemAfter)
		temp = temp->next;

	newP->next = ElemAfter;
	temp->next = newP;
	
	return EXIT_SUCCESS;
}

