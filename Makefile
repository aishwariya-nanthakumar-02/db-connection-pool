CC=gcc
CFLAGS=-Wall -Wextra -lpq -lpthread

all:
	gcc src/main.c src/db_pool.c -o db_pool -Wall -Wextra -lpq -lpthread -I/usr/include/postgresql


clean:
	rm -f db_pool
