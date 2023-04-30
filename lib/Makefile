SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

lib : $(OBJ)
	ar -rcs lib.a $^
	objcopy --redefine-syms=redefine_syms lib.a


%.o : %.c
	gcc -c lib.c

distclean:
	make clean
	rm *.a

clean :
	rm *.o
