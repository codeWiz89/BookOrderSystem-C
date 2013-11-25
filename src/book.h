#ifndef BOOK_H
#define BOOK_H

#endif /* BOOK.H */

#include "uthash.h"

typedef struct bookorder_ bookOrder;
struct bookorder_ {
	char book[500];
	int id;
	float price;
	char category[100];

	UT_hash_handle hh;
};


typedef struct successful_ successfulOrder;
struct successful_ {

	char title[500];
	float price;
	float remaining;
	successfulOrder* next;
};

typedef struct failed_ failedOrder;
struct failed_ {

	char title[500];
	float price;
	failedOrder* next;
};

typedef struct personNode_ personNode;
struct personNode_ {

	char name[500];
	int id;
	float balance;
	char address[500];
	char state[50];
	char zipcode[50];

	successfulOrder* so;
	failedOrder* fo;

	UT_hash_handle hh;
};
