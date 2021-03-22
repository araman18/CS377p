CC=g++
CFLAGS=-std=c++11
SOURCE=main.cpp conversions.cpp
EXEC=pagerank

all: $(EXEC)

$(EXEC):
	$(CC) $(CFLAGS) $(SOURCE) -o $@

clean:
	rm $(EXEC)
