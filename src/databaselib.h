/*
 * databaselib.h
 *
 *  Created on: Mar 16, 2017
 *      Author: tomasz.maczkowski
 */

#ifndef DATABASELIB_H_
#define DATABASELIB_H_

#define NUMPEOPLE 1024
#define NUMNAMES 20
#define NUMSURNAMES 23
#define NELEMS(x) (sizeof(x)/sizeof(*(x)))

#define FILENAME "people.txt"

#define MAXLINE 1024

FILE *pFile;

struct Person *personCreate(char *name, char *surname, int age);
int createPeopleDB();
unsigned hash(char *s);
struct Person *lookUp(struct Person *pplArray[], char *name, char *sur);
struct Person *hashPosition(struct Person *pplArray[], char *sur);

#endif /* DATABASELIB_H_ */
