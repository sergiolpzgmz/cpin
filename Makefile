CC = clang
CFLAGS = -Wall -Wextra -g -Iinclude

all:
	$(CC) $(CFLAGS) src/*.c -o cpin

clean:
	rm -f cpin 

run:
	./cpin 

.PHONY:
	.PHONY: all clean run
