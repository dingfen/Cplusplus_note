CC=gcc
CPP=g++
CFLAGS=
CPPFLAGS=
TARGET=lseek io fcntl

everything : $(TARGET)

lseek: error.c lseek.c
	$(CC) -o $@ $^

io:	error.c io.c
	$(CC) -o $@ $^

fcntl: error.c fcntl.c
	$(CC) -o $@ $^

clean:
	rm $(TARGET)