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

#include "databaselib.h"

static struct Person *peopleDatabase[26];

int flagCreate();
int flagLoad();
int flagDelete(int load);
int flagAge(int load);
int flagPrint(int load);
void saveDB();

int main(int argc, char *argv[])
{

	if (argc < 2) {
		printf("Usable parameters: \n\t-a  :  add new person\n\t-d  :  delete person\n\t"
				"-l  :  load existing databse from file\n\t-c  :  create new database \n\t-p"
				"  :  print entire database\nBefore working on records you have to load database!\n\t");

	}
	int c, flags = 0;

	enum { ADD = 01, DEL = 02, LOAD = 04, CREATE = 010, PRINT = 020 };

	while (--argc > 0 && (*++argv)[0] == '-') {
		while ((c = *++argv[0]))
			switch (c) {
			case 'a':
				flags |= ADD;
				flagAge(flags & LOAD);
				break;
			case 'd':
				flags |= DEL;
				flagDelete(flags & LOAD);
				break;
			case 'l':
				flags |= LOAD;
				flagLoad();
				break;
			case 'c':
				flags |= CREATE;
				return (flagCreate() ? EXIT_SUCCESS : EXIT_FAILURE);
				break;
			case 'p':
				flags |= PRINT;
				flagPrint(flags & LOAD);
				break;
			default:
				printf("unknown parameter: -%c\n", c);
				break;
			}
	}

	saveDB();

	return EXIT_FAILURE;
}

int flagAge(int load)
{
	if (!load) {
			printf("Before printing any record you have to load database!\n");
			exit(EXIT_FAILURE);
	}

	char name[100], surname[100];
	int age;

	struct Person *np;

	printf("Change age mode...\nEnter name, surname and new age : ");
	if ((scanf(" %s %s %d", name, surname, &age)) != 3) {
		printf("Error input data.\n");
		return EXIT_FAILURE;
	}

	if ((np = lookUp(peopleDatabase, name, surname)) == NULL) {
		printf("Can't find person named %s %s.\n", name, surname);
		return EXIT_FAILURE;
	}

	setAge(np, age);

	printf("Age changed.\n");

	return EXIT_SUCCESS;
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

int flagPrint(int load)
{
	if (!load) {
		printf("Before printing any record you have to load database!\n");
		exit(EXIT_FAILURE);
	}

	int i;

	for (i = 0; i < 26; i++)
		if (peopleDatabase[i] != NULL)
			printPerson(peopleDatabase[i]);

	return 0;
}

int flagCreate()
{
	if (!createPeopleDB()) {
		printf("Database creation successful\nCheck file >%s\n", FILENAME);
		return EXIT_SUCCESS;
	} else {
		printf("Error creating database.");
		return EXIT_FAILURE;
	}
}

int flagDelete(int load)
{
	if (!load) {
		printf("Before deleting any record you have to load database!\n");
		exit(EXIT_FAILURE);
	}

	char name[100], surname[100];

	printf("Person killing mode...\nEnter name, surname and new age : ");
	scanf(" %s %s", name, surname);

	peopleDatabase[hash(surname)] = NULL;

	printf("Person decapitation done.\n");

	return EXIT_SUCCESS;
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

			peopleDatabase[hash(surname)] = personCreate(name, surname, age);

			if (peopleDatabase[hash(surname)] == NULL) {
				printf("Error loading db.\n");
				return EXIT_FAILURE;
			}
		}

	}

	fclose(pFile);
	return EXIT_SUCCESS;
}
