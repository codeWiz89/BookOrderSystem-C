#ifndef BOOK_H
#define BOOK_H

#endif /* BOOK.H */

#include "uthash.h"

typedef struct personNode_ personNode;
struct personNode_ {

	char name[100];
	int id;
	double balance;
	char address[100];
	char state[50];
	char zipcode[15];

	UT_hash_handle hh;
};

typedef struct bookorder_ bookOrder;
struct bookorder_ {
	char* book;
	int id;
	double price;
	char* category;
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

