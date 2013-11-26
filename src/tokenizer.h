/*
 * tokenizer.h
 *
 *  Created on: Oct 12, 2013
 *      Author: dev-1
 */

#ifndef TOKENIZER_H_
#define TOKENIZER_H_


#endif /* TOKENIZER_H_ */

struct TokenizerT_ {
	char* copied_string;
	char* delimiters;
	char* current_position;
};

typedef struct TokenizerT_ TokenizerT;


char is_escape_character(char character);

int char_to_hex(char character);

int char_to_oct(char character);

int is_oct_digit(char oct_digit);


char* unescape_string(char* string);

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 *
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *separators, char *ts);

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk);


char is_delimiter(char character, char* delimiters);


/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk);
