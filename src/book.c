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

//	pthread_mutex_lock(&cd_lock);

//	printf("%s\n", str);

	bookOrder* findBookOrder;
	HASH_FIND_STR(bookOrderHead, str, findBookOrder);

	while (findBookOrder != NULL) {

		pthread_mutex_lock(&cd_lock);

		personNode* tempPerson;
		int tempID = findBookOrder->id;
		HASH_FIND_INT(personHead, &tempID, tempPerson);

		if (tempPerson != NULL) {

			printf("%d\n", tempPerson->id);

			if (tempPerson->balance >= findBookOrder->price) {

				int balance = tempPerson->balance;

				if (tempPerson->so == NULL) {

					tempPerson->so = malloc(sizeof(successfulOrder));
					strcpy(tempPerson->so->title, findBookOrder->book);
					tempPerson->so->price = findBookOrder->price;
					tempPerson->so->remaining = balance - findBookOrder->price;
					tempPerson->balance = balance - findBookOrder->price;

				}

				else {

					successfulOrder* prev = NULL;
					successfulOrder* ptr = tempPerson->so;

					successfulOrder* so = malloc(sizeof(successfulOrder));
					strcpy(so->title, findBookOrder->book);
					so->price = findBookOrder->price;
					so->remaining = balance - findBookOrder->price;
					tempPerson->balance = balance - findBookOrder->price;

					do {

						prev = ptr;
						ptr = ptr->next;

					} while (ptr != NULL);

					prev->next = so;
				}

				HASH_DEL(bookOrderHead, findBookOrder);
			}

			else {

				if (tempPerson->fo == NULL) {

					tempPerson->fo = malloc(sizeof(failedOrder));
					strcpy(tempPerson->fo->title, findBookOrder->book);
					tempPerson->fo->price = findBookOrder->price;

				}

				else {

					failedOrder* prev = NULL;
					failedOrder* ptr = tempPerson->fo;

					failedOrder* fo = malloc(sizeof(failedOrder));
					strcpy(fo->title, findBookOrder->book);
					fo->price = findBookOrder->price;

					do {

						prev = ptr;
						ptr = ptr->next;

					} while (ptr != NULL);

					prev->next = fo;
				}

				HASH_DEL(bookOrderHead, findBookOrder);
			}

		}

		HASH_FIND_STR(bookOrderHead, str, findBookOrder);
		pthread_mutex_unlock(&cd_lock);
	}

//	pthread_mutex_unlock(&cd_lock);

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

void writeFinalReport() {

	FILE* dbFile = fopen("finalreport.txt", "w");
	personNode *ptr;
	int i;

	for (ptr = personHead; ptr != NULL; ptr = ptr->hh.next) {

		if (ptr != NULL) {

			fprintf(dbFile, "=== BEGIN CUSTOMER INFO ===\n");
			fprintf(dbFile, "### Balance ###\n");

		//	fprintf(dbFile, "Customer name: %s\n", ptr->name);

			fprintf(dbFile, "Customer name: ");

			for (i = 0; i < strlen(ptr->name); i++) {

				char c = ptr->name[i];

				if (isalpha(c) || c == ' ') {

					fprintf(dbFile, "%c", c);

				}
			}

			fprintf(dbFile, "\n");

			fprintf(dbFile, "Remaining credit balance after all purchases (a dollar amount): %f\n", ptr->balance);
			fprintf(dbFile, "### SUCCESSFUL ORDERS ###\n");

			if (ptr->so != NULL) {

				successfulOrder* sPTR = ptr->so;

				while (sPTR != NULL) {

					fprintf(dbFile, "\"%s\"", sPTR->title);
					fprintf(dbFile, "| %f| ", sPTR->price);
					fprintf(dbFile, "%f", sPTR->remaining);

					fprintf(dbFile, "\n");

					sPTR = sPTR->next;

				}
			}

			fprintf(dbFile, "### REJECTED ORDERS ###\n");

			if (ptr->fo != NULL) {

				failedOrder* fPTR = ptr->fo;

				while (fPTR != NULL) {

					fprintf(dbFile, "\"%s\"", fPTR->title);
					fprintf(dbFile, "| %f", fPTR->price);

					fprintf(dbFile, "\n");

					fPTR = fPTR->next;

				}
			}

			fprintf(dbFile, "### END CUSTOMER INFO ###\n");
			fprintf(dbFile, "\n");
		}
	}

	fclose(dbFile);

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

			//	printDB();
			//	printOrder();
		}

		else {

			printf("Error file cannot be opened...\n");
			return 0;
		}
	}

	pthread_mutex_init(&cd_lock, NULL);

	/*	pthread_t processor_SPORTS01, processor_HOUSING01, processor_POLITICS01;

	 int ret;

	 ret = pthread_create(&processor_SPORTS01, 0, processorThread, cat[0]);
	 //	printf("%d \n", ret);

	 ret = pthread_create(&processor_HOUSING01, 0, processorThread, cat[1]);
	 //	printf("%d \n", ret);

	 ret = pthread_create(&processor_POLITICS01, 0, processorThread, cat[2]);
	 //	printf("%d \n", ret);

	 pthread_join(processor_SPORTS01, 0);
	 pthread_join(processor_HOUSING01, 0);
	 pthread_join(processor_POLITICS01, 0); */

	int threads = count;
	int i;

	pthread_t * thread = malloc(sizeof(pthread_t) * threads);
	int ret;

	for (i = 0; i < threads; i++) {

		ret = pthread_create(&thread[i], NULL, processorThread, cat[i]);

		if (ret != 0) {

			printf("Create pthread error!\n");
			exit(1);
		}
	}

	for (i = 0; i < threads; i++) {

		pthread_join(thread[i], 0);
	}

	printf("\n");
	printf("\n");

	printDB();
	writeFinalReport();

	return 0;
}
