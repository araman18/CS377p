CC=gcc
CFLAGS=-I /usr/lib/x86_64-linux-gnu/libpapi.a /usr/lib/x86_64-linux-gnu/libpfm.so.4
%.o: %.c $(DEPS)
	$(CC) -lpapi -O3 -c -o $@ $< $(CFLAGS)

mmm: main.o
	$(CC) -o mmm main.o

clean:
	rm *.o mmm
