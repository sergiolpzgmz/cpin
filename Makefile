CC = clang
CFLAGS = -Wall -Wextra -g -Iinclude

all:
	$(CC) $(CFLAGS) src/*.c -o cpin

clean:
	rm -f cpin

run:
	./cpin

install: all
	cp cpin /usr/local/bin/cpin

uninstall:
	rm -f /usr/local/bin/cpin

.PHONY: all clean run install uninstall
