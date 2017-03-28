/*
 ============================================================================
 Name        : First.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>

#include "databaselib.h"

static struct Person *peopleDatabase[128];

int dbPtr = 0;

int flagCreate();
int flagLoad();
void saveDB();

int main(int argc, char *argv[])
{
	srand((unsigned int) time(NULL));
	int seeded = 1;

	if (argc == 2 && (*++argv)[0] == '-' && *++argv[0] == 'c')
		return flagCreate(seeded);

	int c, ch, event = 0;

	int age, sex;
	char *name, *surname;

	flagLoad();

	while(1) {

		if ((c = getch()) == 'q')
			break;
		if (c == 'p') {
			int i;
			for (i = 0; i < dbPtr; i++)
				if (peopleDatabase[i] != NULL)
					printPerson(peopleDatabase[i]);
			continue;
		}

		ch = rand() % dbPtr;
		event = rand() % 2;
		if(event) {
			if (peopleDatabase[ch] != NULL) {
				name = strdup(getName(peopleDatabase[ch]));
				surname = strdup(getSurname(peopleDatabase[ch]));
				age = getAge(peopleDatabase[ch]);
				printf("\tDied : %s %s at age of %d\n", name, surname, age);

				personDelete(peopleDatabase[ch]);
				peopleDatabase[ch] = NULL;
			} else {
				sex = rand() % 2; /* 0 - man, 1 - woman */
				name = randName(sex);
				surname = randSurname(sex);
				if (!name || !surname) {
					printf("Error randomizing name / surname\n");
					fclose(pFile);
					return EXIT_FAILURE;
				}
				age = 0;

				peopleDatabase[ch] = personCreate(name, surname, age);

				if (!peopleDatabase[ch]) {
					printf("Error allocating person %d: %s %s\n", ch, getName(peopleDatabase[ch]), getSurname(peopleDatabase[ch]));
					exit(EXIT_FAILURE);
				} else
					printf("\tNew person was born : %s %s\n", name, surname);
			}

		}



		int i;
		for (i = 0; i < dbPtr; i++) {
			if (peopleDatabase[i] != NULL)
				setAge(peopleDatabase[i], getAge(peopleDatabase[i]) + 1);
		}
//		system("cls");
		printf("A year passes...\n");
	}

	saveDB();

	return EXIT_FAILURE;
}

void saveDB()
{
	pFile = fopen(FILENAME, "w+");
	int i;

	for (i = 0; i < 26; i++)
		if (peopleDatabase[i] != NULL)
			writePerson(peopleDatabase[i]);

	fclose(pFile);
}

int flagCreate(int seeded)
{
	if (!createPeopleDB(seeded)) {
		printf("Database creation successful\nCheck file >%s\n", FILENAME);
		return EXIT_SUCCESS;
	} else {
		printf("Error creating database.");
		return EXIT_FAILURE;
	}
}

int flagLoad()
{

	char line[MAXLINE], name[MAXLINE], surname[MAXLINE];

	pFile = fopen(FILENAME, "r");

	int c, i = 0, age;

	while (1) {
		c = fgetc(pFile);

		line[i++] = c;

		if (c == EOF)
			return EXIT_SUCCESS;

		if (c == '\n') {
			sscanf(line, "%s %s %d", name, surname, &age);

//			printf("%s %s %d\n", name, surname, age);

			i = 0;

			peopleDatabase[dbPtr++] = personCreate(name, surname, age);

			if (peopleDatabase[dbPtr - 1] == NULL) {
				printf("Error loading db.\n");
				return EXIT_FAILURE;
			}
		}
	}

	fclose(pFile);
	return EXIT_SUCCESS;
}
