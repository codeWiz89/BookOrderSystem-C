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
#include "tokenizer.h"

#define DELIMS "abcdefghijklmnopqrstuvwyz0123456789";

personNode* personHead = NULL;
bookOrder* bookOrderHead = NULL;

char* cat[50];

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

		HASH_ADD(hh, personHead, id, sizeof(int), toAdd);

		memset(line, 0, 1000);

	}
}

void readOrderFile(FILE* fileToRead) {

	char line[1000] = { 0 };
	FILE* orderFile = fileToRead;
	char* wordToken;
	int count = 1;

	while (!feof(orderFile)) {

		fgets(line, 1000, orderFile);
		bookOrder* toAdd = malloc(sizeof(bookOrder));
		wordToken = strtok(line, "|");
		int i = 0;

		while (wordToken != NULL) {

			if (count == 1) {

				int toInsert = 0;

				for (i = 0; i < strlen(wordToken); i++) {

					char c = wordToken[i];

					if (isalpha(c) || isdigit(c) || c == ' ') {

						toAdd->book[toInsert] = c;
						toInsert++;

					}
				}
			}

			else if (count == 2) {

				toAdd->price = atof(wordToken);

			}

			else if (count == 3) {

				toAdd->id = atoi(wordToken);
			}

			else if (count == 4) {

				int toInsert = 0;

				for (i = 0; i < strlen(wordToken); i++) {

					char c = wordToken[i];

					if (isalpha(c) || isdigit(c)) {

						toAdd->category[toInsert] = c;
						toInsert++;

					}
				}
			}

			count++;
			wordToken = strtok(NULL, "|");
		}

		/*	strcpy(toAdd->book, strtok(line, "|"));
		 toAdd->id = atoi(strtok(NULL, "|"));
		 toAdd->price = atof(strtok(NULL, "|"));
		 strcpy(toAdd->category, strtok(NULL, "|")); */

		HASH_ADD_STR(bookOrderHead, category, toAdd);
		count = 1;
		memset(line, 0, 1000);

	}
}

void *processThread() {

}

void printDB() {

	personNode* temp;

	for (temp = personHead; temp != NULL; temp = temp->hh.next) {

		printf("%s ", temp->name);
		printf("%i ", temp->id);
		printf("%f ", temp->balance);
		printf("%s ", temp->address);
		printf("%s ", temp->state);
		printf("%s\n", temp->zipcode);
	}
}

void printOrder() {

	bookOrder* temp;

	for (temp = bookOrderHead; temp != NULL; temp = temp->hh.next) {

		printf("%s ", temp->book);
		printf("%f ", temp->price);
		printf("%i ", temp->id);
		printf("%s\n", temp->category);
	}
}

int main(int argc, char *argv[]) {

	if (argc < 4 || argc > 4) {

		printf("Illegal number of arguments...");
		return -1;

	}

	char* c = strtok(argv[3], " ");

	int count = 0;

	while (c != NULL) {

		cat[count] = c;
		c = strtok(NULL, " ");
		count += 1;
	}

	if (argv[1] != NULL && argv[2] != NULL) {

		FILE* dbFile = fopen(argv[1], "r");
		FILE* orderFile = fopen(argv[2], "r");

		if (dbFile != NULL) {

			readDBFile(dbFile);
			readOrderFile(orderFile);

			printDB();
			printOrder();
		}

		else {

			printf("Error file cannot be opened...\n");
			return 0;
		}
	}



	/* printf("\n");
	 printf("\n");
	 printf("\n");

	 int i = 0;
	 bookOrder* toFind;

	 for (i = 0; i < count; i++) {

	 HASH_FIND_STR(bookOrderHead, cat[i], toFind);

	 while (toFind != NULL) {

	 printf("%s ", toFind->book);
	 printf("%f ", toFind->price);
	 printf("%i ", toFind->id);
	 printf("%s\n", toFind->category);

	 HASH_DEL(bookOrderHead, toFind);

	 HASH_FIND_STR(bookOrderHead, cat[i], toFind);
	 }
	 }

	 */

	return 0;
}
