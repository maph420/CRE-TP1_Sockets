CC=gcc
CFLAGS=-I -gstabs -Wall

makesock: client.o server.o dep.o
	$(CC) -o client client.o dep.o
	$(CC) -o server server.o dep.o
