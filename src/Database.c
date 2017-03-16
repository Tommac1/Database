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

#include "databaselib.h"

static struct Person *peopleDatabase[26];

int flagCreate();
int flagDelete(int loaded);

int main(int argc, char *argv[])
{

	if (argc < 2) {
		printf("Usable parameters: \n\t-a [NAME] [SURNAME] [AGE]  :  add new person\n\t-d [NAME] [SURNAME] [AGE]  :  delete person\n\t"
				"-l  :  load existing databse from file\n\t-c  :  create new database \n\t-f [PARAM...]"
				"  :  find person with given parameter\n\t");

	}
	int c, flags = 0;
	int i;
chuj
	enum { ADD = 01, DEL = 02, FIND = 04, LOAD = 010, CREATE = 020 };

	while (--argc > 0 && (*++argv)[0] == '-') {
		while (c = *++argv[0])
			switch (c) {
			case 'a':
				flags |= ADD;
				return (flagCreate() ? EXIT_SUCCESS : EXIT_FAILURE);
				break;
			case 'd':
				flags |= DEL;
				break;
			case 'f':
				flags |= FIND;
				break;
			case 'l':
				flags |= LOAD;
				break;
			case 'c':
				flags |= CREATE;
				break;
			default:
				printf("unknown parameter: -%c\n", c);
				break;
			}
	}

	return EXIT_FAILURE;
}

int flagCreate()
{
	if (!createPeopleDB()) {
		printf("Database creation successfull\nCheck file >%s\n", FILENAME);
		return EXIT_SUCCESS;
	} else
		return EXIT_FAILURE;
}

int flagDelete(int loaded)
{
	if (!loaded) {
		printf("Before deleting any record you have to load database!\n");
		exit(EXIT_FAILURE);
	}
	char *name, *surname;

	printf("Insert name and surname : ");


}
