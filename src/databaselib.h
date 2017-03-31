/*
 * databaselib.h
 *
 *  Created on: Mar 16, 2017
 *      Author: tomasz.maczkowski
 */

#ifndef DATABASELIB_H_
#define DATABASELIB_H_

#define NUMPEOPLE 10
#define NUMNAMES 20
#define NUMSURNAMES 23
#define NELEMS(x) (sizeof(x)/sizeof(*(x)))

#define FILENAME "people.txt"

#define MAXLINE 1024

FILE *pFile;

int createPeopleDB(int sranded);
struct Person *personCreate(char *name, char *surname, int age);
void personDelete(struct Person *p);
char *randName(int sex) ;
char *randSurname(int sex);
int randAge(int low, int high);
int writePerson(struct Person *p);
void printPerson(struct Person *p);
struct Person *lookUp(struct Person *pplArray[], char *name, char *sur);
void setAge(struct Person*np, int age);
int getAge(struct Person *np);
char *getName(struct Person *np);
char *getSurname(struct Person *np);
unsigned hash(char *s);

#endif /* DATABASELIB_H_ */
