#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1024

/*
9. Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u čvorove binarnog stabla.
a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraća
pokazivač na korijen stabla.
b) Napisati funkciju replace koja će svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat će se u stablo na
slici Slika 2.
c) Prepraviti program na način da umjesto predefiniranog cjelobrojnog polja korištenjem
funkcije rand() generira slučajne brojeve u rasponu <10, 90>. Također, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon korištenja funkcije iz a), zatim b)
dijela zadatka.
*/

struct MyStruct;
typedef struct MyStruct* pos;
typedef struct MyStruct
{
	int num;
	pos left, right;
}tree;

struct MyStruct2;
typedef struct MyStruct2* Spos;
typedef struct MyStruct2
{
	int Snum;
	Spos next;
}stack;

pos insert(int num, pos root);
pos createEl(num);
int PrintInOrderOnStack(pos root, Spos head);
int replace(pos root);
Spos CreateStackEl(int Snum);
int push(Spos head, int Snum);
int popAll(Spos head);
int printInFile(Spos head);
int appendFile(Spos head);

int main()
{
	pos root = NULL;
	Spos head = CreateStackEl(0);

	int array[10] = { 2, 5, 7, 8,11, 1, 4, 2, 3, 7 };
	int i = 0;
	
	for (i=0; i<10; i++)
		root = insert(array[i], root);

	srand(time(NULL));

	/*for (i = 0; i < 10; i++)
		root = insert((rand() % 81) + 10, root);*/
	
	PrintInOrderOnStack(root, head);
	printInFile(head);
	popAll(head);
	//root->num = replace(root) - root->num;
	replace(root);

	PrintInOrderOnStack(root, head);
	appendFile(head);
	popAll(head);

	return 0;
}

/*int replace(pos root)
{
	static int sum = 0;
	if (root != NULL)
	{
		sum += root->num;
		replace(root->right);
		replace(root->left);
	}

	return sum;
}*/

//ova fja puno bolja od moje
int replace(pos root)
{
	int sum = 0;

	if (root == NULL)
		return sum;

	if (root->left == NULL && root->right == NULL)
	{
		sum = root->num;
		root->num = 0;
		return sum;
	}

	sum += replace(root->left);
	sum += replace(root->right);
	sum += root->num;

	root->num = sum - root->num;

	return sum;
}

int PrintInOrderOnStack(pos root, Spos head)
{
	if (root != NULL)
	{
		PrintInOrderOnStack(root->left, head);
		printf("%d ", root->num);
		push(head, root->num);
		PrintInOrderOnStack(root->right, head);
	}

	return EXIT_SUCCESS;
}

pos insert(int num, pos root)
{
	pos current = root;
	pos NewEl = NULL;

	if (current == NULL)
	{
		return NewEl = createEl(num);
	}
	else if (num > current->num)
	{
		current->left = insert(num, current->left);
	}
	else if (num < current->num)
	{
		current->right = insert(num, current->right);
	}
	else
	{
		current->left = insert(num, current->left);
	}

	return current; // bit ce root
}

pos createEl(int num)
{
	pos NewEl = NULL;

	NewEl = (pos)malloc(sizeof(tree));
	if (NewEl == NULL)
	{
		perror("Malloc greska!");
		return EXIT_FAILURE;
	}

	NewEl->num = num;
	NewEl->left = NULL;
	NewEl->right= NULL;

	return NewEl;
}

Spos CreateStackEl(int Snum)
{
	Spos NewEl = NULL;
	NewEl = (Spos)malloc(sizeof(stack));
	if (NewEl == NULL)
	{
		perror("Malloc greska!");
		return EXIT_FAILURE;
	}
	
	NewEl->Snum = Snum;
	NewEl->next = NULL;

	return NewEl;
		
}

int push(Spos head, int Snum)
{
	Spos NewEl;
	NewEl = CreateStackEl(Snum);

	NewEl->next = head->next;
	head->next = NewEl;

	return EXIT_SUCCESS;
}

int popAll(Spos head)
{
	Spos temp = NULL;

	while (head->next != NULL)
	{
		temp = head->next;
		head->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

int printInFile(Spos head)
{
	FILE* fp = NULL;
	Spos temp = head->next;

	fp = fopen("stablo.txt", "w");
	
	while (temp)
	{
		fprintf(fp, "%d ", temp->Snum);
		temp = temp->next;
	}
	fprintf(fp, "\n");

	fclose(fp);

	return EXIT_SUCCESS;
}

int appendFile(Spos head)
{
	FILE* fp = NULL;
	Spos temp = head->next;

	fp = fopen("stablo.txt", "a");

	while (temp)
	{
		fprintf(fp, "%d ", temp->Snum);
		temp = temp->next;
	}
	fprintf(fp, "\n");

	fclose(fp);

	return EXIT_SUCCESS;
}