#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 128

struct MyStruct;
typedef struct MyStruct* posTree;
typedef struct MyStruct {
	char city[MAX];
	int population;
	posTree left;
	posTree right;
} tree;

struct MyStruct2;
typedef struct MyStruct2* posList;
typedef struct MyStruct2 {
	char country[MAX];
	posTree cities;
	posList next;
} list;

posList CreateNewListEl();
posList FindCountry(posList, char*);
posTree CreatNewTreeEl();

int ReadCountryFile(posList, char*);
int AddCountryInList(posList, char*, char*);
posTree AddCityInTree(posTree, char*, int);
posTree ReadCityFile(posTree, char*);
int PrintCountryCities(posList);
int InOrderPrintTree(posTree);

int Choose(posList);
int FindCitiesBiggerThan(posTree, int);

int main() {

	//a
	//koristen reference kod
	int bzv = 0;

	posList head = NULL;

	head = CreateNewListEl();

	ReadCountryFile(head, "drzave.txt");

	PrintCountryCities(head->next);

	Choose(head);
	return EXIT_SUCCESS;
}

posList FindCountry(posList head, char* country) {
	while (head != NULL) {
		if (strcmp(country, head->country) == 0) {
			return head;
		}
		else {
			head = head->next;
		}
	}
	return NULL;
}

int Choose(posList head) {
	posList q = NULL;
	char tmp1[MAX];
	int tmp2;
	printf("Odaberi ime drzave koju zelis pretrazivati: ");
	scanf(" %s", tmp1);
	printf("Nadi sve gradove vece od x stanovnika: ");
	scanf("%d", &tmp2);
	q = FindCountry(head, tmp1);
	if (!q) {
		printf("Zadana drzava ne postoji!");
		return EXIT_FAILURE;
	}
	FindCitiesBiggerThan(q->cities, tmp2);
	return EXIT_SUCCESS;
}

int FindCitiesBiggerThan(posTree p, int populationX) {
	if (p == NULL) {
		return EXIT_SUCCESS;
	}

	FindCitiesBiggerThan(p->left, populationX);

	if (p->population > populationX) {
		printf("%s %d, ", p->city, p->population);
	}
	FindCitiesBiggerThan(p->right, populationX);

	return EXIT_SUCCESS;
}

int PrintCountryCities(posList head) 
{
	while (head != NULL) {
		printf("%s gradovi: \n", head->country);
		InOrderPrintTree(head->cities);
		printf("\n\n");
		head = head->next;
	}
	return EXIT_SUCCESS;
}

posList CreateNewListEl() {
	posList q = malloc(sizeof(list));
	if (q == NULL) {
		printf("Malloc greska!");
		return NULL;
	}
	strcpy(q->country, "");
	q->cities = NULL;
	q->next = NULL;
	return q;
}

posTree CreatNewTreeEl(char* city, int population) {
	posTree q = malloc(sizeof(tree));
	if (q == NULL) {
		printf("Malloc greska!");
		return NULL;
	}
	strcpy(q->city, city);
	q->population = population;
	q->left = NULL;
	q->right = NULL;
	return q;
}

int InOrderPrintTree(posTree p) {
	if (p == NULL) {
		return EXIT_SUCCESS;
	}
	else {
		InOrderPrintTree(p->left);
		printf("%s %d, ", p->city, p->population);
		InOrderPrintTree(p->right);
	}
	return EXIT_SUCCESS;
}

int ReadCountryFile(posList head, char* FileName) {
	char CountryName[MAX] = "", CityFileName[MAX] = "";

	FILE* dat = NULL;
	dat = fopen(FileName, "r");
	if (!dat) {
		printf("Greska tijekom otvaranja datoteke!");
		return EXIT_FAILURE;
	}

	while (!feof(dat)) {
		fscanf(dat, "%s %s", CountryName, CityFileName);
		AddCountryInList(head, CountryName, CityFileName);
	}
	fclose(dat);
	return EXIT_SUCCESS;
}

int AddCountryInList(posList head, char* country, char* CityFileName) {
	posList tmp = NULL;
	char tmp1[MAX], tmp2[MAX];

	tmp = CreateNewListEl();
	strcpy(tmp->country, country);

	tmp->cities = ReadCityFile(tmp->cities, CityFileName);

	//abecedno unosenje u listu
	strcpy(tmp1, country);
	while (head->next != NULL) {
		strcpy(tmp2, head->next->country);
		if (strcmp(tmp1, tmp2) > 0) {
			head = head->next;
			continue;
		}
		break;
	}

	tmp->next = head->next;
	head->next = tmp;

	return EXIT_SUCCESS;
}

posTree ReadCityFile(posTree root, char* FileName) {
	char cityName[MAX] = "";
	int cityPopulation = 0;
	FILE* dat = NULL;

	dat = fopen(FileName, "r");
	if (!dat) {
		printf("Greska pri otvaranju datoteke!");
		return NULL;
	}

	while (!feof(dat)) {
		fscanf(dat, "%s %d", cityName, &cityPopulation);
		root = AddCityInTree(root, cityName, cityPopulation);
	}
	fclose(dat);
	return root;
}

posTree AddCityInTree(posTree p, char* city, int population) {
	if (p == NULL) {
		p = CreatNewTreeEl(city, population);
	}
	else if (population == p->population) {
		if (strcmp(city, p->city) < 0) {
			p->left = AddCityInTree(p->left, city, population);
		}
		if (strcmp(city, p->city) > 0) {
			p->right = AddCityInTree(p->right, city, population);
		}
	}
	else {
		if (population < p->population) {
			p->left = AddCityInTree(p->left, city, population);
		}
		if (population > p->population) {
			p->right = AddCityInTree(p->right, city, population);
		}
	}
	return p;
}