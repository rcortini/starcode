P=starcode
OBJECTS= trie.o starcode.o
CFLAGS= -std=c99 -Wall -g -Wall -O3
LDLIBS= -lm
CC= gcc
$(P): $(OBJECTS) main.c

#all: $(P) tquery

#tquery: tquery.c starcode.h trie.o
#	$(CC) $(CFLAGS) $(LDLIBS) tquery.c trie.o -o tquery

clean:
	rm -f $(P) $(OBJECTS) tquery
