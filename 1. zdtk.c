#include <stdio.h> 
#include <stdlib.h>


#define MAX_SIZE (128)
#define MAX_LINE (1024)

typedef struct _student
{
	char ime[MAX_SIZE];
	char prezime[MAX_SIZE];
	double bodovi;
	} student; 

int brRed(char* nazDat);
student* ucitavanjeStud(int brojStudenata, char* nazDat);
void ispis (student* studenti, int brStud, int najBodovi);
double najRezultat(student* studenti, int brStud);

int main()
{
	int brStud = 0;
    double najBodovi = 0;
	char nazDat[MAX_SIZE] = "studenti.txt";
    student *studenti = NULL;

	/*printf("Ime datoteke: ");
    scanf(" %s", nazDat);*/

	brStud = brRed(nazDat);
	studenti = ucitavanjeStud(brStud, nazDat);
	najBodovi= najRezultat(studenti, brStud);
	ispis(studenti, brStud, najBodovi);
 
	return 0;
}

int brRed(char* nazDat)
{
	int brojac = 0;
	FILE* datoteka = NULL;
	char buffer[MAX_LINE] = {0};


	datoteka = fopen(nazDat, "r");
	if (!datoteka)
	{
		printf("Neuspjelo otvaranje datoteke!");
		return -1;
	}
	
	while (!feof(datoteka))
	{
		fgets(buffer, MAX_LINE, datoteka);
		brojac++;
	}
	
	fclose(datoteka);

	return brojac;
}

student* ucitavanjeStud(int brojStudenata, char* nazDat)
{
	int brojac = 0;
	FILE* datoteka = NULL;
	student* studenti = NULL;
	
	studenti = (student*)malloc(brojStudenata * sizeof(student));
	datoteka = fopen(nazDat, "r");
	if (!datoteka)
	{
		printf("Neuspjelo otvaranje datoteke!");
		free(studenti);
		return NULL;
	}

	while (!feof(datoteka))
	{
		fscanf(datoteka, " %s\t\t%s\t\t%lf", /*namjerno space jer brise ono sta je bilo ispred*/
			studenti[brojac].ime,
			studenti[brojac].prezime,
			&studenti[brojac].bodovi);
		brojac++;
	}
	
	fclose(datoteka);

	return studenti;
}

void ispis (student* studenti, int brStud, int najBodovi)
{
	int i=0;
	printf("IME\t\tPREZIME\t\tBODOVI (50 max)\tRELATIVNI BODOVI (100 max)\n");

	for (i=0;i<brStud;i++)
		{
			printf("%-15s %-15s %.2f %16.2f\n", studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, studenti[i].bodovi/najBodovi*100);
		}
}

double najRezultat(student* studenti, int brStud)
{
	int i=0;
	double najBodovi=0;

	najBodovi= studenti[0].bodovi;
	for(i=1;i<brStud;i++)
		if(studenti[0].bodovi > najBodovi)
			najBodovi=studenti[i].bodovi;

	return najBodovi;
}

