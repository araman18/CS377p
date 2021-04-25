CC=g++
CFLAGS=-pthread -std=c++11
SOURCE=simple.cpp
EXEC=pagerank

: $(EXEC)

$(EXEC):
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCE) -o $@

clean:
	rm $(EXEC)
