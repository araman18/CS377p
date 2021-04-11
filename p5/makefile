CC=g++
CFLAGS=-pthread -std=c++11
SOURCE=arrayCompute.cpp
EXEC=computePi

: $(EXEC)

$(EXEC):
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCE) -o $@

clean:
	rm $(EXEC)
