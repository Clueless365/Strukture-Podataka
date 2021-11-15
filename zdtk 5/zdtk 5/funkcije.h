#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FUNKCIJE_H
#define FUNKCIJE_H

struct _StackElement;
typedef struct _StackElement* Position;
typedef struct _StackElement {
	double num;
	Position next;
} StackElement;

int menu(double* result);
Position CreateElement(double num);
int InsertAfter(Position position, Position newElement);
int Push(Position head, double num);
int DeleteAfter(Position position);
int Pop(double* result, Position head); //gledamo je li exit succes zato int ,a saljemo adresu rezultata
int PerformOperation(Position head, char operation);
int CalculatePostfixFromFile(double* result, char* fileName);


#endif