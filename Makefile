CC ?= cc
CFLAGS ?= -O2 -Wall -Wextra -Werror -std=c11
dirwatch: dirwatch.c
	$(CC) $(CFLAGS) dirwatch.c -o dirwatch
test: dirwatch
	python3 tests/integration.py
clean:
	rm -f dirwatch
.PHONY: test clean
