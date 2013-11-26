COMPILER = gcc
CCFLAGS = -Wall
all:book

debug:
	make DEBUG=TRUE


book.o: book.c uthash.h
	$(COMPILER) $(CCFLAGS) -c book.c -lpthread

book: book.o tokenizer.o
	$(COMPILER) $(CCFLAGS) -o book book.o tokenizer.o -lpthread

tokenizer.o: tokenizer.c tokenizer.h
	$(COMPILER) $(CCFLAGS) -c tokenizer.c

ifeq ($(DEBUG), TRUE)
 CCFLAGS += -g

endif



clean:
	rm -f book
	rm -f *.o

