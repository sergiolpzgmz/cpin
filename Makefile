CC = clang
CFLAGS = -Wall -Wextra -g -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L -Iinclude

all:
	$(CC) $(CFLAGS) src/*.c -o cpin

clean:
	rm -f cpin cpin_asan

run:
	./cpin

asan:
	$(CC) $(CFLAGS) -fsanitize=address,undefined -fno-omit-frame-pointer src/*.c -o cpin_asan

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./cpin

install: all
	cp cpin /usr/local/bin/cpin

uninstall:
	rm -f /usr/local/bin/cpin

.PHONY: all asan valgrind clean run install uninstall
