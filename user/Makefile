#Makefile

FILE = thread
NAME = $(FILE)

all: $(FILE).c
	gcc $(FILE).c -o $(NAME) -O1 -lpthread

run:
	./$(NAME)

:phony
clean:
	rm ./$(NAME)
	rm -f *.o
