CC=g++
CFLAGS=-pthread -std=c++11
SOURCE=pi.cpp
EXEC=computePi

: $(EXEC)

$(EXEC):
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCE) -o $@

clean:
	rm $(EXEC)
