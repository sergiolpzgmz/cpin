CC = clang
CFLAGS = -Wall -Wextra -g -Iinclude

all:
	$(CC) $(CFLAGS) src/*.c -o cpin

asan:
	$(CC) $(CFLAGS) -fsanitize=address,undefined -fno-omit-frame-pointer src/*.c -o cpin_asan

clean:
	rm -f cpin cpin_asan

run:
	./cpin

install: all
	cp cpin /usr/local/bin/cpin

uninstall:
	rm -f /usr/local/bin/cpin

.PHONY: all asan clean run install uninstall
