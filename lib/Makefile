SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

lib : $(OBJ)
	ar -rcs lib.a $^

%.o : %.c
	gcc -c $^

distclean:
	make clean
	rm *.a

clean :
	rm *.o
