CC=gcc
CFLAGS=-Wall -Werror -std=gnu99 -g -I. -fsanitize=address
LDFLAGS=-fsanitize=address

TEST_SRC=$(wildcard tests/*.c)
TEST_OBJ=$(TEST_SRC:.c=.o)
TEST_EXEC=$(TEST_SRC:.c=)

.PHONY: clean

all: $(TEST_EXEC) bio.o

printvars:
	@echo $(TEST_SRC)
	@echo $(TEST_EXEC)

bio.o: bio.h bio.c

$(TEST_EXEC): %: %.o bio.o
	$(CC) $(LDFLAGS) -o $@ $< bio.o

$(TEST_OBJ): %.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@ 

%.o: %.c bio.h bio.c
	$(CC) $(CFLAGS) -c $< -o $@

test: all
	@./runtests.sh

clean:
	rm -rf $(TEST_OBJ) $(TEST_EXEC) *.o 
	
