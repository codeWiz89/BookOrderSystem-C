COMPILER = gcc
CCFLAGS = -Wall
all:book

debug:
	make DEBUG=TRUE


book.o: book.c uthash.h
	$(COMPILER) $(CCFLAGS) -c book.c -lpthread

book: book.o 
	$(COMPILER) $(CCFLAGS) -o book book.o -lpthread


ifeq ($(DEBUG), TRUE)
 CCFLAGS += -g

endif



clean:
	rm -f book
	rm -f *.o
