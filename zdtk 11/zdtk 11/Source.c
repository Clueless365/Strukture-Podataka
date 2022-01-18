#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 128
#define MAX_HASH_TABLE_SIZE 11

struct MyStruct;
typedef struct MyStruct* posTree;
typedef struct MyStruct 
{
	char city[MAX];
	int population;
	posTree left;
	posTree right;

} tree;

struct MyStruct2;
typedef struct MyStruct2* posList;
typedef struct MyStruct2 
{
	char country[MAX];
	posTree cities;
	posList next;

} list;

struct MyStruct3;
typedef struct MyStruct3* PosHashTab;
typedef struct MyStruct3 
{
	posList* headList;

} HashTable;

posList CreateNewListEl();
posList FindCountry(PosHashTab, char*);
posTree CreatNewTreeEl();

int AddCountryInHashTable(PosHashTab, char*, char*);
int ReadCountryFile(PosHashTab, char*);
int AddCountryInList(posList, char*, char*);
posTree AddCityInTree(posTree, char*, int);
posTree ReadCityFile(posTree, char*);
int PrintCountryCities(posList);
int InOrderPrintTree(posTree);

int Odaberi(PosHashTab);
int FindCitiesBiggerThan(posTree, int);
PosHashTab MakeHashTab();
int HashFunction(char* );
int Choose(PosHashTab);

int main() {

	//koristen reference kod
	PosHashTab hashTab = NULL;

	hashTab = MakeHashTab();

	ReadCountryFile(hashTab, "drzave.txt");

	Choose(hashTab);

	return EXIT_SUCCESS;
}

int HashFunction(char* CountryName) 
{
	int result = 0;
	unsigned int i = 0;
	//zaustavi petlju ako je manje od 5 charova ili ime drzave krace
	for (i = 0; i < strlen(CountryName) || i < 5; i++)
	{
		result += CountryName[i];
	}
	return result % MAX_HASH_TABLE_SIZE;
}

PosHashTab MakeHashTab() 
{
	PosHashTab hash = NULL;
	int i = 0;

	hash = (PosHashTab)malloc(sizeof(HashTable));
	if (hash == NULL)
	{
		printf("Malloc greska!");
		return NULL;
	}
	hash->headList = (posList*)malloc(sizeof(posList) * MAX_HASH_TABLE_SIZE);

	if (hash->headList == NULL) 
	{
		printf("Malloc greska!");
		return NULL;
	}

	for (i = 0; i < MAX_HASH_TABLE_SIZE; i++) 
	{
		hash->headList[i] = CreateNewListEl();
	}
	return hash;
}

int AddCountryInHashTable(PosHashTab hashTab, char* CountryName, char* CitiesFileName) 
{
	int  index = 0;
	index = HashFunction(CountryName);

	AddCountryInList(hashTab->headList[index], CountryName, CitiesFileName);

	return EXIT_SUCCESS;
}


int Choose(PosHashTab hashTab) 
{
	posList q = NULL;
	char temp1[MAX];
	int temp2;
	int check = 1;

	printf("Odaberi ime drzave koju zelis pretrazivati (Veliko slovo je bitno!):\n");
	check=scanf(" %s", temp1);
	if (check == 0)
	{
		perror("Scanf greska!");
		return EXIT_FAILURE;
	}
	printf("Nadi sve gradove vece od x stanovnika: ");

	check=scanf("%d", &temp2);
	if (check == 0)
	{
		perror("Scanf greska!");
		return EXIT_FAILURE;
	}

	q = FindCountry(hashTab, temp1);

	if (!q)
	{
		printf("Zadana drzava ne postoji!");
		return EXIT_FAILURE;
	}
	FindCitiesBiggerThan(q->cities, temp2);

	return EXIT_SUCCESS;
}

int FindCitiesBiggerThan(posTree p, int SearchedPopulation) 
{
	if (p == NULL) 
	{
		return EXIT_SUCCESS;
	}

	FindCitiesBiggerThan(p->left, SearchedPopulation);

	if (p->population > SearchedPopulation) 
	{
		printf("%s %d, ", p->city, p->population);
	}
	FindCitiesBiggerThan(p->right, SearchedPopulation);

	return EXIT_SUCCESS;
}

posList CreateNewListEl() 
{
	posList q = malloc(sizeof(list));
	if (q == NULL)
	{
		printf("Malloc greska!");
		return NULL;
	}

	strcpy(q->country, "");
	q->cities = NULL;
	q->next = NULL;
	return q;
}

posTree CreatNewTreeEl(char* city, int population) 
{
	posTree q = malloc(sizeof(tree));
	if (q == NULL) 
	{
		printf("Malloc greska!");
		return NULL;
	}
	strcpy(q->city, city);
	q->population = population;
	q->left = NULL;
	q->right = NULL;
	return q;
}

int InOrderPrintTree(posTree p) 
{
	if (p == NULL) 
	{
		return EXIT_SUCCESS;
	}
	else 
	{
		InOrderPrintTree(p->left);
		printf("%s %d, ", p->city, p->population);
		InOrderPrintTree(p->right);
	}
	return EXIT_SUCCESS;
}

int ReadCountryFile(PosHashTab hashTab, char* FileName) 
{
	char CountryName[MAX] = "", CitiesFileName[MAX] = "";
	int check = 1;

	FILE* fp = NULL;
	fp = fopen(FileName, "r");
	if (!fp) {
		printf("Greska tijekom otvaranju datoteke");
		return EXIT_FAILURE;
	}

	while (!feof(fp))
	{
		check = fscanf(fp, "%s %s", CountryName, CitiesFileName);
		if (check == 0)
		{
			perror("Fscanf greska!");
			return EXIT_FAILURE;
		}
		AddCountryInHashTable(hashTab, CountryName, CitiesFileName);
	}
	fclose(fp);
	return EXIT_SUCCESS;
}

int AddCountryInList(posList headList, char* country, char* CitiesFileName) 
{
	posList temp = NULL;
	char temp1[MAX], temp2[MAX];

	temp = CreateNewListEl();
	strcpy(temp->country, country);

	temp->cities = ReadCityFile(temp->cities, CitiesFileName);

	//abecedno unošenje u listu
	strcpy(temp1, country);

	while (headList->next != NULL) 
	{
		strcpy(temp2, headList->next->country);
		if (strcmp(temp1, temp2) > 0) {
			headList = headList->next;
			continue;
		}
		break;
	}

	temp->next = headList->next;
	headList->next = temp;

	return EXIT_SUCCESS;
}

posTree ReadCityFile(posTree root, char* FileName) 
{
	char CityName[MAX] = "";
	int CityPopulation = 0;
	FILE* fp = NULL;
	int check = 1;

	fp = fopen(FileName, "r");
	if (!fp) 
	{
		printf("Greska pri otvaranju datoteke");
		return NULL;
	}

	while (!feof(fp)) 
	{
		check=fscanf(fp, "%s %d", CityName, &CityPopulation);
		if (check == 0)
		{
			perror("Fscanf greska!");
			return NULL;
		}
		root = AddCityInTree(root, CityName, CityPopulation);
	}
	fclose(fp);
	return root;
}

posTree AddCityInTree(posTree p, char* city, int population)
{
	if (p == NULL) 
		p = CreatNewTreeEl(city, population);

	else if (population < p->population)
		p->left = AddCityInTree(p->left, city, population);

	else if (population > p->population)
		p->right = AddCityInTree(p->right, city, population);

	else
	{
		if (strcmp(city, p->city) < 0)		
			p->left = AddCityInTree(p->left, city, population);
		
		else
			p->right = AddCityInTree(p->right, city, population);
	}
		
	return p;
}

posList FindCountry(PosHashTab hashTab, char* country) 
{
	int index = HashFunction(country);
	posList headList = hashTab->headList[index];

	while (headList->next != NULL) 
	{
		if (strcmp(country, headList->next->country) == 0) 
		{
			return headList->next;
		}
		else 
		{
			headList = headList->next;
		}
	}
	return NULL;
}