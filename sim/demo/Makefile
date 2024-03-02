all: digit

# include needed for OS-X
CFLAGS = -I/usr/X11/include

#CFLAGS = -DDEBUG

clean:
	rm -f *.o *~ calc

digit_7x9.o: digit_7x9.c digit_7x9.h

digit_7x9_cb.o: digit_7x9_cb.c digit_7x9.h

digit_7x9_main.o: digit_7x9_main.c digit_7x9.h

digit: digit_7x9.o digit_7x9_cb.o digit_7x9_main.o
	cc -o digit $^ -lforms
