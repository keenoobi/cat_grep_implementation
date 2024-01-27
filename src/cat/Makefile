CC= gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra
NAME= s21_cat



all:$(NAME)

s21_cat: $(NAME).c
	$(CC) $^ $(CFLAGS) -o $(NAME)

test: rebuild 
	sh test_cat.sh

clang:
	clang-format -style=Google -n *.c *.h

rebuild: clean all

clean:
	rm -rf $(NAME)