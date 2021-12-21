#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define MAX 128
#define MAX_LINE 1024

struct MyStruct;
typedef struct MyStruct* pos;
typedef struct MyStruct
{
	char name[MAX];
	pos parent;
	pos sibling;
	pos child;
}tree;
//tree predstavlja direktorije, root je C: direktorij,
//svaki direktorij ima jednog childa, osim ako je na dnu stabla onda je NULL
//te svaki direktorij ima parenta osim root-a, a sibling funkcioniraju kao stack
//prvi napravljeni direktorij nema siblinga,  a sljedeci ce pokazivat na prvog 
//treci na drugog i tako dalje dok se ne napravi novi level i onda ispocetka
struct MyStruct2;
typedef struct MyStruct2* Spos;
typedef struct MyStruct2
{
	char Sname[MAX_LINE];
	Spos next;
	
}stack;
/*Ova struktura predstavlja trnutunu lokaciju, svaki put kad se pozove cd,
ako je uspjesan, na kraj liste se stavlja ime direktorija, ako se pozove cdBack
kraj se brise*/
int displayLocation(Spos head);
pos createDir(pos NewDir, char* name);
pos makeRootDir();
Spos createStackEl(char* name);
Spos findLastStackEl(Spos head);
int md(char* name, pos current);
pos cd(char* name, pos current, Spos head);
pos cdBack(pos current, Spos head);
int dir(pos current);
Spos createHeadStackEl();
int deleteLastStackEl(Spos head);
int menu(pos current, Spos head);

int main()
{
	pos root = NULL;
	Spos head = NULL;
	root = makeRootDir();
	head = createHeadStackEl();

	menu(root, head);

	return 0;
}


pos createDir(pos NewDir, char* name)
{
	NewDir = (pos)malloc(sizeof(tree));
	if (NewDir == NULL) {
		perror("Malloc greska!");
		return EXIT_FAILURE;
	}
	strcpy(NewDir->name, name);
	NewDir->parent = NULL;
	NewDir->sibling = NULL;
	NewDir->child = NULL;

	return NewDir;
}


pos makeRootDir()
{
	pos NewDir = NULL;
	NewDir = (pos)malloc(sizeof(tree));
	if (NewDir == NULL) {
		perror("Malloc greska!");
		return EXIT_FAILURE;
	}

	strcpy(NewDir->name, "C:/");
	NewDir->parent = NULL;
	NewDir->sibling = NULL;
	NewDir->child = NULL;

	return NewDir;
}
Spos createStackEl(char* name)
{
	Spos NewEl = NULL;
	NewEl = (Spos)malloc(sizeof(stack));
	if (NewEl == NULL) {
		perror("Malloc greska!");
		return EXIT_FAILURE;
	}

	strcpy(NewEl->Sname, "/");
	strcat(NewEl->Sname, name);
	//strcat(NewEl->Sname, " ");

	NewEl->next = NULL;

	return NewEl;
}
Spos findLastStackEl(Spos head)
{
	Spos temp = head;

	while (temp->next != NULL)
		temp = temp->next;

	return temp;
}

int md(char* name, pos current)
{
	pos temp = current;
	pos NewDir = NULL;

	if (temp->child == NULL) {
		NewDir = createDir(NewDir, name);
		temp->child = NewDir;
		NewDir->parent = temp;

		return EXIT_SUCCESS;
	}
	else if (temp->child != NULL) {
		NewDir = createDir(NewDir, name);
		NewDir->sibling = temp->child;
		//temp->child->parent = NULL;
		temp->child = NewDir;
		NewDir->parent = temp;
	}

	return EXIT_SUCCESS;
}

pos cd(char* name, pos current, Spos head)
{
	pos temp = NULL;
	Spos last = NULL;
	Spos newLast = NULL;

	if (current->child == NULL) {
		printf("Prazan direktorij!\n");
		return current;
	}

	else if (strcmp(current->child->name, name) == 0) {
		current = current->child;

		last = findLastStackEl(head);
		newLast = createStackEl(name);
		newLast->next = last->next;
		last->next = newLast;

		return current;
	}

	temp = current->child;
	temp = temp->sibling;

	while (temp)
	{
		if (strcmp(temp->name, name) == 0)
		{
			current = temp;

			last = findLastStackEl(head);
			newLast = createStackEl(name);
			newLast->next = last->next;
			last->next = newLast;

			return current;
		}
		else
			temp = temp->sibling;
	}

	printf("Nema tog dir-a\n");
	return current;

}

pos cdBack(pos current, Spos head)
{
	Spos temp = head;
	
	if (current->parent == NULL)
	{
		printf("Dosli ste do root dir-a\n");
		return current;
	}

	current = current->parent;

	deleteLastStackEl(temp);

	return current;
}

int dir(pos current)
{
	printf("==============================\n");
	pos temp = NULL;
	if (current->child == NULL) {
		printf("==============================\n");
		return EXIT_SUCCESS;
	}

	temp = current->child;

	while (temp != NULL)
	{
		printf("%s\n", temp->name);

		temp = temp->sibling;
	}
	printf("==============================\n");
	return EXIT_SUCCESS;
}

Spos createHeadStackEl()
{
	Spos NewEl = NULL;
	NewEl = (Spos)malloc(sizeof(stack));
	if (NewEl == NULL) {
		perror("Malloc greska!");
		return EXIT_FAILURE;
	}

	strcpy(NewEl->Sname, ">C:/");
	NewEl->next = NULL;

	return NewEl;
}


int deleteLastStackEl(Spos head)
{
	Spos temp = head;
	Spos last = NULL;
	while (temp->next->next != NULL)
		temp = temp->next;

	last = temp->next;
	temp->next = NULL;
	free(last);

	return EXIT_SUCCESS;
}

int menu(pos root, Spos head)
{
	pos current = root;
	Spos temp = head;
	char name[MAX_LINE] = { 0 };
	char command[MAX_LINE] = { 0 };
	char line[MAX_LINE] = { 0 };
	printf(">C:/ ");


	while (1)
	{
		fgets(line, MAX_LINE, stdin);
		sscanf(line, "%s %s", command, name);

		if (!strcmp(command, "md"))
		{
			md(name, current);
			displayLocation(temp);
			printf(" ");
			
			//printf("Current= %s\nCurrent->child=%s\n",current->name,current->child->name);
		}
		else if (!strcmp(command, "cd"))
		{
			current = cd(name, current, temp);
			displayLocation(temp);
			printf(" ");
		}
		else if (!strcmp(command, "cd.."))
		{
			current = cdBack(current, temp);
			displayLocation(temp);
			printf(" ");

		}
		else if (!strcmp(command, "dir"))
		{
			dir(current);
			displayLocation(temp);
			printf(" ");
		}
		else if (!strcmp(command, "exit"))
		{		
			break;
		}
		else
		{
			printf("'%s' nije prepoznano kao naredba!\n", command);
		}
	}
	return EXIT_SUCCESS;
}
int displayLocation(Spos head)
{
	Spos temp=head;

	while (temp)
	{
		printf("%s", temp->Sname);
		temp = temp->next;
	}

	return EXIT_SUCCESS;
}