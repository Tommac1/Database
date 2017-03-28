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

//FILE *pFile;

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
		{ "Maria", "Krystyna", "Anna", "Barbara", "Teresa", "Elzbieta", "Janina", "Zofia", "Jadwiga", "Danuta", "Halina", "Irena",
		  "Ewa", "Malgorzata", "Helena", "Grazyna", "Bozena", "Stanislawa", "Jolanta", "Marianna" },
		{ "Jan", "Stanislaw", "Andrzej", "Jozef", "Tadeusz", "Jerzy", "Zbigniew", "Krzysztof", "Henryk", "Ryszard", "Kazimierz",
		  "Marek", "Marian", "Piotr", "Janusz", "Wladyslaw", "Adam", "Wieslaw", "Zdzislaw", "Edward" }
};

const char *surnames[2][NUMSURNAMES] = {
		{ "Nowak", "Kowalska", "Wisniewska", "Dabrowska", "Lewandowska", "Wojcik", "Kaminska", "Kowalczyk", "Zielinska",
		  "Szymanska", "Wozniak", "Kozlowska", "Jankowska", "Wojciechowska", "Kwiatkowska", "Kaczmarek", "Mazur",
		  "Krawczyk", "Piotrowska", "Grabowska", "Nowakowska", "Pawlowska", "Michalska" },
		{ "Nowak", "Kowalski", "Wisniewski", "Dabrowski", "Lewandowski", "Wojcik", "Kaminski", "Kowalczyk", "Zielinski",
		  "Szymanski", "Wozniak", "Kozlowski", "Jankowski", "Wojciechowski", "Kwiatkowski", "Kaczmarek", "Mazur",
		  "Krawczyk", "Piotrowski", "Grabowski", "Nowakowski", "Pawlowski", "Michalski" }
};

int createPeopleDB(int sranded)
{
	pFile = fopen(FILENAME, "w+");

	int nPer, randSex, rAge;
	char *rName, *rSurname;
	struct Person *np;

	if (!sranded) srand((unsigned int) time(NULL)); /* unsigned int to get rid of compilator warning */

	for (nPer = 0; nPer < NUMPEOPLE; nPer++) {
		randSex = rand() % 2; /* 0 - man, 1 - woman */
		rName = randName(randSex);
		rSurname = randSurname(randSex);
		if (!rName || !rSurname) {
			printf("Error randomizing name / surname\n");
			fclose(pFile);
			return EXIT_FAILURE;
		}
		rAge = randAge(0, 90);

		np = personCreate(rName, rSurname, rAge);

		if (!np) {
			printf("Error allocating person %d: %s %s\n", nPer, np->name, np->surname);
			exit(EXIT_FAILURE);
		}

		if (!fprintf(pFile, "%s %s %d\n", np->name, np->surname, np->age)) {
			printf("Error writing person %d to file\n", nPer);
			fclose(pFile);
			return EXIT_FAILURE;
		}
	}
	free(np);
	fclose(pFile);

	return EXIT_SUCCESS;
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

void personDelete(struct Person *p)
{
	free(p->name);
	free(p->surname);
	free(p);
	p = NULL;
}

char *randName(int sex)
{
	char *rName;
	rName = strdup(names[sex][rand() % NUMNAMES]);
	return rName;
}

char *randSurname(int sex)
{
	char *rsName;
	rsName = strdup(surnames[sex][rand() % NUMNAMES]);
	return rsName;
}

int randAge(int low, int high)
{
	int rAge = (rand() % (high - low + 1)) + low;
	return rAge;
}

int writePerson(struct Person *p)
{
	pFile = fopen(FILENAME, "a+");
	if (!fprintf(pFile, "%s %s %d\n", p->name, p->surname, p->age)) {
		printf("Error writing person to file\nDid you create the database with -c parameter?\n");
		fclose(pFile);
		free(p);
		return EXIT_FAILURE;
	} else {
		//printf("Adding person successful.");
		free(p);
		fclose(pFile);
		return EXIT_SUCCESS;
	}
}

void printPerson(struct Person *p)
{
	char *name = getName(p);
	char *surname = getSurname(p);
	int age = getAge(p);

	printf("%s %s %d\n", name, surname, age);
}

struct Person *lookUp(struct Person *pplArray[], char *name, char *sur)
{
	struct Person *pPerson;

	for (pPerson = pplArray[hash(sur)]; pPerson != NULL; pPerson = pPerson->next)
		if ((strcmp(sur, pPerson->surname) == 0) && (strcmp(name, pPerson->name) == 0))
			return pPerson;

	return pPerson;
}

void setAge(struct Person*np, int age)
{
	np->age = age;
}

int getAge(struct Person *np)
{
	return np->age;
}

char *getName(struct Person *np)
{
	return np->name;
}

char *getSurname(struct Person *np)
{
	return np->surname;
}

unsigned hash(char *s)
{
	return *s - 'A';
}
