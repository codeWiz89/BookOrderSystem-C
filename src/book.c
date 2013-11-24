/*
 *  book.c
 *
 *  Created on: Nov 20, 2013
 *  Author: Inderjeet Bhattal, Charlie Shin
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <pthread.h>
#include "book.h"

personNode* personHead = NULL;
bookOrder* bookOrderHead = NULL;

void readDBFile(FILE* fileToRead) {

	char line[1000] = { 0 };
	FILE* dbFile = fileToRead;

	while (!feof(dbFile)) {

		fgets(line, 1000, dbFile);

		personNode* toAdd = malloc(sizeof(personNode));

		strcpy(toAdd->name, strtok(line, "|"));
		toAdd->id = atoi(strtok(NULL, "|"));
		toAdd->balance = atof(strtok(NULL, "|"));
		strcpy(toAdd->address, strtok(NULL, "|"));
		strcpy(toAdd->state, strtok(NULL, "|"));
		strcpy(toAdd->zipcode, strtok(NULL, "|"));

		HASH_ADD_STR(personHead, name, toAdd);

		memset(line, 0, 1000);

	}
}

void readOrderFile(FILE* fileToRead) {

	char line[1000] = { 0 };
	FILE* dbFile = fileToRead;

	while (!feof(dbFile)) {

		fgets(line, 1000, dbFile);



	}
}

void printDB() {

	personNode* temp;

	for (temp = personHead; temp != NULL; temp = temp->hh.next) {

		printf("%s", temp->name);
		printf("%d", temp->id);
		printf("%f", temp->balance);
		printf("%s", temp->address);
		printf("%s", temp->state);
		printf("%s\n", temp->zipcode);
	}
}

/*void freeAll() {

	personNode *ptr, tmp;

	HASH_ITER(hh, personHead, ptr, tmp) {

		HASH_DEL(personHead, ptr); // delete it (words advances to next)

		free(ptr->name);
		free(ptr->address);
		free(ptr->state);

		free(ptr);// free it
	}
}

*/

int main(int argc, char *argv[]) {

	if (argc < 4 || argc > 4) {

		printf("Illegal number of arguments...");
		return -1;

	}

	char fileName[sizeof(argv[1])];
	strcpy(fileName, argv[1]);

	if (fileName != NULL) {

		FILE* dbFile = fopen(fileName, "r");

		if (dbFile != NULL) {

			readDBFile(dbFile);
			printDB();
		}

		else {

			printf("Error file cannot be opened...\n");
			return 0;
		}
	}

	return 0;
}
