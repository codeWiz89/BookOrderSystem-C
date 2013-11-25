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
pthread_mutex_t cd_lock;

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

void *processorThread(void *arg) {

	char *str = (char*) arg;

	/*	pthread_mutex_lock(&cd_lock); // "Dont touch the queue! I'm changing it."

	 //	printf("%s\n", str);

	 bookOrder* toFind;
	 HASH_FIND_STR(bookOrderHead, str, toFind);

	 while (toFind != NULL) {

	 //	printf("%s ", toFind->book);
	 //	printf("%f ", toFind->price);
	 //	printf("%i ", toFind->id);
	 //	printf("%s\n", toFind->category);

	 HASH_DEL(bookOrderHead, toFind);
	 HASH_FIND_STR(bookOrderHead, str, toFind);
	 }

	 pthread_mutex_unlock(&cd_lock); */

	pthread_mutex_lock(&cd_lock); // "Dont touch the queue! I'm changing it."

	bookOrder* findOrder;
	HASH_FIND_STR(bookOrderHead, str, findOrder);

	if (findOrder != NULL) {

		personNode* tempPerson;
		int tempID = findOrder->id;

		HASH_FIND_INT(personHead, &tempID, tempPerson);

		if (tempPerson != NULL) {

			if (tempPerson->balance >= findOrder->price) {

				int balance = tempPerson->balance;

				if (tempPerson->so == NULL) {

					tempPerson->so = malloc(sizeof(successfulOrder));
					strcpy(tempPerson->so->title, findOrder->book);
					tempPerson->so->price = findOrder->price;
					tempPerson->so->remaining = balance - findOrder->price;
					tempPerson->balance = balance - findOrder->price;

				}

				else {

					successfulOrder* prev = NULL;
					successfulOrder* ptr = tempPerson->so;

					successfulOrder* so = malloc(sizeof(successfulOrder));
					strcpy(so->title, findOrder->book);
					so->price = findOrder->price;
					so->remaining = balance - findOrder->price;
					tempPerson->balance = balance - findOrder->price;

					do {

						prev = ptr;
						ptr = ptr->next;

					} while (ptr != NULL);

					prev->next = so;
				}

				HASH_DEL(bookOrderHead, findOrder);
			}

			else {

				failedOrder* fo = malloc(sizeof(failedOrder));
				strcpy(fo->title, findOrder->book);
				fo->price = findOrder->price;

				HASH_DEL(bookOrderHead, findOrder);

				if (tempPerson->fo == NULL) {

					tempPerson->fo = malloc(sizeof(failedOrder));
					strcpy(tempPerson->fo->title, findOrder->book);
					tempPerson->fo->price = findOrder->price;

				}

				else {

					failedOrder* prev = NULL;
					failedOrder* ptr = tempPerson->fo;

					failedOrder* fo = malloc(sizeof(failedOrder));
					strcpy(fo->title, findOrder->book);
					fo->price = findOrder->price;

					do {

						prev = ptr;
						ptr = ptr->next;

					} while (ptr != NULL);

					prev->next = fo;
				}
			}
		}
	}

	pthread_mutex_unlock(&cd_lock);

	return 0;

}


void printDB() {

	personNode* temp;

	for (temp = personHead; temp != NULL; temp = temp->hh.next) {

		if (temp != NULL) {

			printf("%s ", temp->name);
			printf("%i ", temp->id);
			printf("%f ", temp->balance);
			printf("%s ", temp->address);
			printf("%s ", temp->state);
			printf("%s\n", temp->zipcode);

			if (temp->so != NULL) {

				successfulOrder* ptr = temp->so;

				while (ptr != NULL) {

					printf("%s ", ptr->title);
					printf("%f ", ptr->price);
					printf("%f \n", ptr->remaining);

					ptr = ptr->next;

				}
			}

			if (temp->fo != NULL) {

				printf("Now Printing failed orders...\n");

				failedOrder* ptr = temp->fo;

				while (ptr != NULL) {

					printf("%s ", ptr->title);
					printf("%f \n", ptr->price);

					ptr = ptr->next;

				}
			}
		}

		printf("\n");
		printf("\n");

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

	pthread_mutex_init(&cd_lock, NULL);

	pthread_t processor_SPORTS01, processor_HOUSING01, processor_POLITICS01;

	int ret;

	ret = pthread_create(&processor_SPORTS01, NULL, processorThread, cat[0]);
	printf("%d \n", ret);

	ret = pthread_create(&processor_HOUSING01, NULL, processorThread, cat[1]);
	printf("%d \n", ret);

	ret = pthread_create(&processor_POLITICS01, NULL, processorThread, cat[2]);
	printf("%d \n", ret);

	pthread_join(processor_SPORTS01, NULL);
	pthread_join(processor_HOUSING01, NULL);
	pthread_join(processor_POLITICS01, NULL);

	printDB();

	printf("\n");
	printf("\n");
	printf("\n");

	printOrder();




	return 0;
}
