all:ww

first:ww.o
	gcc -g -Wall -Werror -fsanitize=address -std=c11  -o ww ww.o -lm

clean:
	rm -rf ww.o ww
	
