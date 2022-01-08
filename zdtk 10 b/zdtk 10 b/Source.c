#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 128

struct MyStruct;
typedef struct MyStruct* posList;
typedef struct MyStruct
{
	char city[MAX];
	int population;
	posList next;

} list;

struct MyStruct2;
typedef struct MyStruct2* posTree;
typedef struct MyStruct2
{
	char country[MAX];
	posTree left;
	posTree right;
	posList cities;

} tree;

int PrintList(posList head);
int InOrderPrint(posTree root);
posTree CreatNewTreeEl(char* name, posList head);
posTree ReadCountriesFile();
posTree AddCountryInTree(posTree root, char* country, posList head);
posList ReadCitiesFile(char* CityFileName);
posList CreatNewListEl(char* city, int population);
int FindCountry(char* CountryName, posTree root);
int PrintGreaterpopulationCities(posList head);

int main()
{
	//original kod, country fileovi copy paste

	posTree root = NULL;
	char buffer[MAX];
	root = ReadCountriesFile();

	InOrderPrint(root);

	printf("Upisi drzavu koju zelis pretraziti: (veliko pocetno slovo je bitno!): ");
	fgets(buffer, MAX, stdin);

	buffer[strcspn(buffer, "\n")] = 0;

	FindCountry(buffer, root);

	printf("\n");

	return EXIT_SUCCESS;
}

int PrintGreaterpopulationCities(posList head)
{
	int check = -1;
	int choice = -2;
	posList temp = head->next;
	printf("Trazite grad sa vise stanovnika od:");
	check = scanf(" %d", &choice);
	if (check == -1)
	{
		perror("Scanf greska!");
		return EXIT_FAILURE;
	}
	while (temp)
	{
		if (temp->population > choice)
			printf("%s %d, ", temp->city, temp->population);

		temp = temp->next;
	}

	return EXIT_SUCCESS;
}

int FindCountry(char* CountryName, posTree root)
{
	if (strcmp(CountryName, root->country) == 0) {
		PrintGreaterpopulationCities(root->cities);
		return EXIT_SUCCESS;
	}

	else if (strcmp(CountryName, root->country) > 0)
		FindCountry(CountryName, root->right);

	else if ((strcmp(CountryName, root->country) < 0))
		FindCountry(CountryName, root->left);

	else {
		printf("Zemlja nije pronadena ili je krivo upisana!");
		return EXIT_FAILURE;
	}
			
	return EXIT_SUCCESS;
}

int PrintList(posList head)
{
	posList temp = head->next;
	while (temp)
	{
		printf("%s: %d, ", temp->city, temp->population);
		temp = temp->next;
	}
	printf("\n");

	return EXIT_SUCCESS;
}

int InOrderPrint(posTree root)
{
	if (root == NULL)
		return EXIT_SUCCESS;

	InOrderPrint(root->left);
	printf("%s \n", root->country);
	PrintList(root->cities);
	InOrderPrint(root->right);

	return EXIT_SUCCESS;
}

posTree CreatNewTreeEl(char* name, posList head)
{
	posTree NewEl = NULL;
	NewEl = malloc(sizeof(tree));

	if (NewEl == NULL)
	{
		perror("Malloc greska!");
		return EXIT_FAILURE;
	}

	strcpy(NewEl->country, name);
	NewEl->left = NULL;
	NewEl->right = NULL;
	NewEl->cities = head;

	return NewEl;
}

//citaj glavni file
posTree ReadCountriesFile()
{
	char country[MAX];
	char CitiesFile[MAX];
	posTree root = NULL;
	posTree el = NULL;
	posList tempHead = NULL;
	int check = 0;
	

	FILE* fp = NULL;
	fp = fopen("drzave.txt", "r");
	
	if (fp == NULL)
	{
		perror("Greska u otvaranju datoteke!");
		return EXIT_FAILURE;
	}

	while (!feof(fp))
	{
		//procitaj drzavu i ime filea u kojem su gradovi drzave
		check = fscanf(fp, " %s %s", country, CitiesFile);
		if (check == 0)
		{
			perror("Fscnaf greska!");
			return EXIT_FAILURE;
		}
		
		tempHead = ReadCitiesFile(CitiesFile);
		root = AddCountryInTree(root, country, tempHead);
	}
	
	fclose(fp);

	return root;
}

posTree AddCountryInTree(posTree root, char* country, posList head)
{
	if (root == NULL)
		return  CreatNewTreeEl(country, head);

	else if (strcmp(country, root->country) > 0)
		root->right = AddCountryInTree(root->right, country, head);

	else if (strcmp(country, root->country) < 0)
		root->left = AddCountryInTree(root->left,  country, head);

	return root;
	
}

posList ReadCitiesFile(char* CityFileName)
{
	posList head = CreatNewListEl("", 0);
	posList temp = NULL;
	posList NewEl = NULL;
	char city[MAX] = "";
	int population = 0;
	int check = 0;

	FILE* fp = NULL;

	fp = fopen(CityFileName, "r");
	if (fp == NULL)
	{
		perror("Greska kod otvaranja datoteke!");
		return EXIT_FAILURE;
	}
	//dodavanje u listu
	while (!feof(fp))
	{
		check = fscanf(fp, " %s %d", city, &population);
		if (check == 0)
		{
			perror("Fscnaf greska!");
			return EXIT_FAILURE;
		}
		temp = head;
		while (temp->next!=NULL)
		{
			//ako je populacija novog manja onda nextaj
			if (temp->next->population > population)
			{
				temp = temp->next;
				continue;
			}
			//dok su populacije novog i starog iste i dok novi ima vece slovo po ASCII 
			while(temp->next!=NULL && temp->next->population == population
					&& strcmp(city, temp->next->city) > 0)
					temp = temp->next;

			//breakaj ako populacija novog nije manja ili ne dodemo do kraja 
			//(i ovi while sortira po abcd ako imaju iste popul)
			break;
		}
		NewEl = CreatNewListEl(city, population);
		NewEl->next = temp->next;
		temp->next = NewEl;
	}

	fclose(fp);

	return head;
}

posList CreatNewListEl(char* city, int population)
{
	posList NewEl = NULL;
	NewEl = malloc(sizeof(list));
	if (NewEl == NULL)
	{
		perror("Malloc greska!");
		return EXIT_FAILURE;
	}

	strcpy(NewEl->city, city);
	NewEl->population = population;
	NewEl->next = NULL;

	return NewEl;
}