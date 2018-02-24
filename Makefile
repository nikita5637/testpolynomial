CC=g++
CFLAGS=-std=c++11

run: install
	clear && ./a.out
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp
polynomial.o: polynomial.cpp
	$(CC) $(CFLAGS) polynomial.cpp
install:
	clear && $(CC) $(CFLAGS) main.cpp polynomial.cpp -o a.out
clean:
	rm a.out
