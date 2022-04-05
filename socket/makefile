CC=gcc
CFLAGS=-I -gstabs -Wall

makesock: client.o server.o dep.o
	$(CC) -g -o client client.o dep.o
	$(CC) -g -o server server.o dep.o
