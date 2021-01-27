CC=gcc
CFLAGS=-I.
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mmm: main.o
	$(CC) -o mmm main.o
	
clean:
	rm *.o mmm
