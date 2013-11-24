#ifndef BOOK_H
#define BOOK_H

#endif /* BOOK.H */

#include "uthash.h"

typedef struct personNode_ personNode;
struct personNode_ {

	char name[500];
	int id;
	double balance;
	char address[500];
	char state[50];
	char zipcode[50];

	UT_hash_handle hh;
};

typedef struct bookorder_ bookOrder;
struct bookorder_ {
	char book[500];
	int id;
	double price;
	char category[100];

	UT_hash_handle hh;
};


typedef struct successful_ successful;
struct successful_ {
	char* title;
	double price;
	double remaining;
	successful* next;
};

typedef struct failed_* failed;
struct failed_ {
	char* title;
	double price;
	failed* next;
};

