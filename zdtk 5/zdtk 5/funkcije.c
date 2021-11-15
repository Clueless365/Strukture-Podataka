#define _CRT_SECURE_NO_WARNINGS
#include "funkcije.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 128

int menu(double* result)
{
	char FileName[MAX] = { 0 };
	int status=0;
	

	printf("Upisite naziv datoteke(BEZ sufiksa .txt)\n"
			"ili ostavite prazno za izlaz iz programa(default ime. postfix): ");
	
	while (1)
	{
		*result = 0;
		fgets(FileName, MAX, stdin);
		
		if (FileName[0] == '\n')
			return EXIT_SUCCESS;

		FileName[strcspn(FileName, "\n")] = 0;//skidanje /n-a

		strcat(FileName, ".txt");

		status = CalculatePostfixFromFile(result, FileName);
		if (status != 0)
		{
			perror("Greska!\n");
			return EXIT_FAILURE;
		}
		printf("Rezultat je: %.2lf\n\n"
			"Ostavite prazno za izlaz ili upisite naziv nove datoteke: ", *result);

	}

}




Position CreateElement(double num)
{
	Position newEl = NULL;

	newEl = (Position)malloc(sizeof(StackElement));

	if (!newEl)
	{
		perror("Malloc greska!\n");
		return NULL;
	}

	newEl->num = num;
	newEl->next = NULL;
	return newEl;
}

int InsertAfter(Position position, Position newEl)
{
	newEl->next = position->next;
	position->next = newEl;

	return EXIT_SUCCESS;
}

int Push(Position head, double num)
{
	Position newEl = NULL;

	newEl = CreateElement(num);

	if (!newEl)
		return -1;

	InsertAfter(head, newEl);

	return EXIT_SUCCESS;
}

int DeleteAfter(Position position)
{
	Position temp = position->next;

	if (!temp)
		return EXIT_SUCCESS;

	position->next = temp->next;
	free(temp);

	return EXIT_SUCCESS;
}

int Pop(double* result, Position head)
{
	Position first = head->next;
	if (!first)
	{
		perror("Postfix ne valja, provjerite datoteku!\n");
		return -1;
	}

	*result = first->num;
	DeleteAfter(head);

	return EXIT_SUCCESS;
}

int PerformOperation(Position head, char operation)
{
	double op1 = 0;
	double op2 = 0;
	double result = 0;
	int status1 = 0;
	int status2 = 0;

	status2 = Pop(&op2, head);
	if (status2 != EXIT_SUCCESS)
		return -1;

	status1 = Pop(&op1, head);
	if (status1 != EXIT_SUCCESS)
		return -2;

	switch (operation)
	{
	case '+':
		result = op1 + op2;
		break;

	case '-':
		result = op1 - op2;
		break;

	case '*':
		result = op1 * op2;
		break;

	case '/':
		if (op2 == 0)
		{
			printf("Zabranjeno je dijeljenje s 0\n");
			return -3;
		}
		result = op1 / op2;
		break;
	default:
		printf("Ova operacija nije jos podrzana (greska)\n");
		return -4;
		break;
	}

	Push(head, result);

	return EXIT_SUCCESS;
}

int CalculatePostfixFromFile(double* result, char* fileName)
{
	FILE* file = NULL;
	int fileLength = 0;
	char* buffer = NULL;
	char* currentBuffer = 0;
	char operation = 0;
	double num = 0;
	int numBytes = 0;
	int status = 0;
	StackElement head = { .num = 0, .next = NULL };

	file = fopen(fileName, "rb"); //citaj u binarnom obliku (puno brze, nema redaka 01101)

	if (!file)
	{
		perror("Fopen greska, provjerite datoteku ili njeno ime!\n");
		return -1;
	}

	fseek(file, 0, SEEK_END);//pronadi kraj datoteke
	fileLength = ftell(file);//kolika je duzina datoteke

	buffer = (char*)calloc(fileLength + 1, sizeof(char));//broj elem za aloc i vel elem

	if (!buffer)
	{
		perror("Calloc greska!\n");
		return -1;
	}

	rewind(file);//vracanje na pocetak datoteke
	fread(buffer, sizeof(char), fileLength, file);//u buffer za el te velicine itd.
	printf("|%s|\n", buffer);
	fclose(file);

	currentBuffer = buffer;
	while (strlen(currentBuffer) > 0)
	{
		status = sscanf(currentBuffer, " %lf %n", &num, &numBytes);

		if (status == 1)
		{
			Push(&head, num);
			currentBuffer += numBytes;
		}

		else
		{
			sscanf(currentBuffer, " %c %n", &operation, &numBytes);
			status = PerformOperation(&head, operation);

			if (status != EXIT_SUCCESS)
			{
				free(buffer);
				while (head.next)// delete all
				{
					DeleteAfter(&head);
				}

				return -2;
			}

			currentBuffer += numBytes;
		}
	}

	free(buffer);
	status = Pop(result, &head);

	if (status != EXIT_SUCCESS)
		return -3;
	if (head.next != NULL)
	{
		printf("Postfix ne valja, provjerite datoteku!\n\n");
		//msm da bi tribalo delete allž
		while (head.next)// delete all
		{
			DeleteAfter(&head);
		}
		return -4;
	}
	
	return EXIT_SUCCESS;
}
