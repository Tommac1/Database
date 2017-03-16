/*
 * databaselib.c
 *
 *  Created on: Mar 16, 2017
 *      Author: tomasz.maczkowski
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "databaselib.h"

FILE *pFile;

struct Person {
	struct Person *next;
	char *name;
	char *surname;
	int age;
};

struct Person *personCreate(char *name, char *surname, int age);
int createPeopleDB();
unsigned hash(char *s);

const char *names[2][NUMNAMES] = {
		{ "Maria", "Krystyna", "Anna", "Barbara", "Teresa", "Elżbieta", "Janina", "Zofia", "Jadwiga", "Danuta", "Halina", "Irena",
		  "Ewa", "Małgorzata", "Helena", "Grażyna", "Bożena", "Stanisława", "Jolanta", "Marianna" },
		{ "Jan", "Stanisław", "Andrzej", "Józef", "Tadeusz", "Jerzy", "Zbigniew", "Krzysztof", "Henryk", "Ryszard", "Kazimierz",
		  "Marek", "Marian", "Piotr", "Janusz", "Władysław", "Adam", "Wiesław", "Zdzisław", "Edward" }
};

const char *surnames[2][NUMSURNAMES] = {
		{ "Nowak", "Kowalska", "Wisniewska", "Dabrowska", "Lewandowska", "Wojcik", "Kaminska", "Kowalczyk", "Zielinska",
		  "Szymanska", "Wozniak", "Kozlowska", "Jankowska", "Wojciechowska", "Kwiatkowska", "Kaczmarek", "Mazur",
		  "Krawczyk", "Piotrowska", "Grabowska", "Nowakowska", "Pawlowska", "Michalska" },
		{ "Nowak", "Kowalski", "Wisniewski", "Dabrowski", "Lewandowski", "Wojcik", "Kaminski", "Kowalczyk", "Zielinski",
		  "Szymanski", "Wozniak", "Kozlowski", "Jankowski", "Wojciechowski", "Kwiatkowski", "Kaczmarek", "Mazur",
		  "Krawczyk", "Piotrowski", "Grabowski", "Nowakowski", "Pawlowski", "Michalski" }
};

int createPeopleDB()
{
	pFile = fopen(FILENAME, "w+");

	int nPer, randSex, randAge;
	char *randName, *randSurname;
	struct Person *np;

	srand((unsigned int) time(NULL)); /* unsigned int to get rid of compilator warning */

	for (nPer = 0; nPer < NUMPEOPLE; nPer++) {
		randSex = rand() % 2; /* 0 - man, 1 - woman */
		randName = strdup(names[randSex][rand() % NUMNAMES]);
		randSurname = strdup(surnames[randSex][rand() % NUMSURNAMES]);
		if (!randName || !randSurname) {
			printf("Error randomizing name / surname\n");
			fclose(pFile);
			return 1;
		}
		randAge = (rand() % 51) + 20; /* age range 20-70 */

		np = personCreate(randName, randSurname, randAge);

		if (!np) {
			printf("Error allocating person %d: %s %s\n", nPer, np->name, np->surname);
			exit(EXIT_FAILURE);
		}

		if (!fprintf(pFile, "%s %s %d\n", np->name, np->surname, np->age)) {
			printf("Error writing person %d to file\n", nPer);
			fclose(pFile);
			return 1;
		}
	}
	free(np);
	fclose(pFile);

	return 0;
}

struct Person *personCreate(char *name, char *surname, int age)
{
	struct Person *np;

	np = (struct Person*) malloc(sizeof(struct Person));

	if (!np || !(np->name = strdup(name)) || !(np->surname = strdup(surname))) {
		printf("Error creating person \"%s %s\"\n", name, surname);
		return NULL;
	}

	np->age = age;

	return np;
}

struct Person *lookUp(struct Person *pplArray[], char *s, char param)
{
	struct Person *pPerson;

	if (param == 'n') {
		for (pPerson = pplArray[hash(s)]; pPerson != NULL; pPerson = pPerson->next)
			if (strcmp(s, pPerson->name) == 0)
				return pPerson;
	} else if (param == 's') {
		for (pPerson = pplArray[hash(s)]; pPerson != NULL; pPerson = pPerson->next)
			if (strcmp(s, pPerson->surname) == 0)
				return pPerson;
	} else {
		printf("unknown lookUp param %c\n", param);
		exit(EXIT_FAILURE);
	}
	return NULL;
}

int loadPerson(struct Person *pplArray[], struct Person *person) {

}

unsigned hash(char *s)
{
	return *s - 'A';
}
