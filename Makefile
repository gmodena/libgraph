C=gcc
CFLAGS= -g
SOURCES=bc.c lib/stack.c lib/centrality.c lib/graph.c lib/graphio.c
HEADERS=lib/stack.h lib/centrality.h lib/graph.h lib/graphio.h
DESTS=$(SOURCES:.c=.o)


all: cb

.c.o:
	$(CC) $(CFLAFS) $(DEFINES) -c $< -o $@

bc:  $(DESTS)
	$(CC) $(CFLAGS) $(DESTS) -o bc

clean:
	rm -f bc lib/*.o lib/*~ *.o 
			
