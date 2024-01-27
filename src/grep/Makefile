CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra
NAME= s21_grep



all:$(NAME)

s21_grep: $(NAME).c
	$(CC) $^ $(CFLAGS) -o $(NAME)

test: rebuild 
	sh test_grep.sh

clang:
	clang-format -style=Google -n *.c *.h
	
rebuild: clean all

clean:
	rm -rf $(NAME)
