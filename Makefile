CC = gcc
CFLAGS = -Wall

all: Servers Equipament

Servers: Servers.c
	$(CC) $(CFLAGS) -o Servers Servers.c

Equipament: Equipament.c
	$(CC) $(CFLAGS) -o Equipament Equipament.c

.PHONY: clean

clean:
	rm -f Servers Equipament
