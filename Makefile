CC = clang
CFLAGS = -Wall -Wextra -g -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L -Iinclude

all:
	$(CC) $(CFLAGS) src/*.c -o cpin

clean:
	rm -f cpin

run:
	./cpin

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./cpin

install: all
	cp cpin /usr/local/bin/cpin

uninstall:
	rm -f /usr/local/bin/cpin

.PHONY: all clean run install uninstall
