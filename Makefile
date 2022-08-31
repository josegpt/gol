CFLAGS=	-O2 -Wall -Werror -Wextra -pedantic -ansi

all: gol

.c.o:
	${CC} ${CFLAGS} -c $<

gol: gol.o
	${CC} -o $@ $?

clean:
	rm *.o gol

.PHONY: all clean
